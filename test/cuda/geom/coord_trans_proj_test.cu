#include "../../../thirdparty/catch.hpp"
#include "../../cpp/geom/proj_geom_params_test_fix.h"
#include "../../../src/cuda/geom/coord_trans_proj.cuh"
#include "../../../src/cuda/geom/ct_geom_dev.cuh"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"

namespace GCT::Test
{
TEST_CASE("Detector coordinate is computed from the pixel coordinate", "[geom][coord_trans_proj]")
{
    using namespace Geom::CUDA;
    SECTION("Returns 5.0") {
        REQUIRE(DCSToPCS(4.0f, 2.0f, 3.0f) == Approx{5.0f});
    }
    SECTION("Returns 1.0") {
        REQUIRE(DCSToPCS(-4.0f, 2.0f, 3.0f) == Approx{1.0f});
    }
}

TEST_CASE("Pixel coordinate is computed from the detector coordinate", "[geom][coord_trans_proj]")
{
    using namespace Geom::CUDA;
    SECTION("Returns 4.0") {
        REQUIRE(PCSToDCS(5u, 2.0f, 3.0f) == Approx{4.0f});
        REQUIRE(PCSToDCS(5.0f, 2.0f, 3.0f) == Approx{4.0f});
    }
    SECTION("Returns -4.0f") {
        REQUIRE(PCSToDCS(1u, 2.0f, 3.0f) == Approx{-4.0f});
        REQUIRE(PCSToDCS(1.0f, 2.0f, 3.0f) == Approx{-4.0f});
    }
}

TEST_CASE("Detector coordinate (axial) of parallel projection is computed",
          "[geom][coord_trans_proj]")
{
    using namespace Geom::CUDA;
    std::floating_point
    auto constexpr pi_f = std::numbers::pi_v<float>;
    SECTION("World origin is mapped to 0.0") {
        REQUIRE(detCoordAxialParallel(0.0f, 0.0f, 0.0f) == Approx{0.0f});
    }
    SECTION("At angle 0 degree, returns y-coordinate (3.0f)") {
        REQUIRE(detCoordAxialParallel(0.0f, 1.0f, 3.0f) == Approx{3.0f});
    }
    SECTION("At angle 90 degree, returns negative of x-coordinate (-1.0f)") {
        REQUIRE(detCoordAxialParallel(pi_f / 2.0f, 1.0f, 3.0f) == Approx{-1.0f});
    }
}

TEST_CASE_METHOD(Parallel1DTestParams,
                 "Projected detector coordinate is computed for 1D parallel projection",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetParallel;

    auto pg = std::vector<ProjGeom<DetRow>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Geom::Utils::wrapAngle(pg.at(proj_idx).rot_ang.val) == Approx{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float2(0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(),
                                           proj_idx, backproj_weight);

        REQUIRE(det_coord == Approx{0.0f});
        REQUIRE(backproj_weight == Approx{1.0f});
    }

    SECTION("(2.0, 0.0) is mapped to 2.0f") {
        auto const pix_pos_wcs = make_float2(2.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const
            det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(), proj_idx, backproj_weight);
        REQUIRE(det_coord == Approx{2.0f});
        REQUIRE(backproj_weight == Approx{1.0f});
    }

    SECTION("(-2.0, 0.0) is mapped to -2.0f") {
        auto const pix_pos_wcs = make_float2(-2.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const
            det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(), proj_idx, backproj_weight);
        REQUIRE(det_coord == Approx{-2.0f});
        REQUIRE(backproj_weight == Approx{1.0f});
    }
}

TEST_CASE_METHOD(FanBeamTestParams,
                 "Projected detector coordinate is computed for fan-beam projection (arc detector)",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetFanArc;

    auto pg = std::vector<ProjGeom<DetRow>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Detail::rotAng(pg.at(proj_idx).src_pos) == Units::Radians{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float2(0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const
            det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(), proj_idx, backproj_weight);
        REQUIRE(det_coord == Approx{0.0f});
        //REQUIRE(backproj_weight == Approx{1.0f / pg.at(proj_idx).src_iso_dist_mm});
    }

    SECTION("The sign of detector coordinate is correctly computed") {
        auto backproj_weight = 0.0f;
        auto const pix_pos_pos = make_float2(2.0f, 0.0f);
        auto const det_coord_pos = projMapping(pix_pos_pos, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);

        auto const pix_pos_neg = make_float2(-2.0f, 0.0f);
        auto const det_coord_neg = projMapping(pix_pos_neg, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);
        REQUIRE(det_coord_pos > det_coord_neg);
        REQUIRE(det_coord_pos == Approx{std::fabs(det_coord_neg)});
    }
}

TEST_CASE_METHOD(FanBeamTestParams,
                 "Projected detector coordinate is computed for fan-beam projection (line detector)",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetFanLine;

    auto pg = std::vector<ProjGeom<DetRow>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Detail::rotAng(pg.at(proj_idx).src_pos) == Units::Radians{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float2(0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(),
                                           proj_idx, backproj_weight);

        REQUIRE(det_coord == Approx{0.0f});
        //REQUIRE(backproj_weight == Approx{pg.at(proj_idx).src_det_dist_mm /
        //    pg.at(proj_idx).src_iso_dist_mm});
    }

    SECTION("The sign of detector coordinate is correctly computed") {
        auto backproj_weight = 0.0f;
        auto const pix_pos_pos = make_float2(2.0f, 0.0f);
        auto const det_coord_pos = projMapping(pix_pos_pos, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);

        auto const pix_pos_neg = make_float2(-2.0f, 0.0f);
        auto const det_coord_neg = projMapping(pix_pos_neg, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);
        REQUIRE(det_coord_pos > det_coord_neg);
        REQUIRE(det_coord_pos == Approx{std::fabs(det_coord_neg)});
    }
}

TEST_CASE_METHOD(Parallel2DTestParams,
                 "Projected detector coordinate is computed for 2D parallel projection",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetParallel;
    using DetCol = DetParallel;

    auto pg = std::vector<ProjGeom<DetRow, DetCol>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow, DetCol>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Geom::Utils::wrapAngle(pg.at(proj_idx).rot_ang.val) == Approx{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f},
                                              DetCol{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float3(0.0f, 0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(),
                                           proj_idx, backproj_weight);

        REQUIRE(det_coord.x == Approx{0.0f});
        REQUIRE(det_coord.y == Approx{0.0f});
        REQUIRE(backproj_weight == Approx{1.0f});
    }

    SECTION("The sign of detector coordinate is correctly computed") {
        auto backproj_weight = 0.0f;
        auto const pix_pos_pos = make_float3(2.0f, 0.0f, 2.0f);
        auto const det_coord_pos = projMapping(pix_pos_pos, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);

        auto const pix_pos_neg = make_float3(-2.0f, 0.0f, -2.0f);
        auto const det_coord_neg = projMapping(pix_pos_neg, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);
        REQUIRE(det_coord_pos.x > det_coord_neg.x);
        REQUIRE(det_coord_pos.y < det_coord_neg.y);
        REQUIRE(std::fabs(det_coord_pos.x) == Approx{std::fabs(det_coord_neg.x)});
        REQUIRE(std::fabs(det_coord_pos.y) == Approx{std::fabs(det_coord_neg.y)});
    }
}

TEST_CASE_METHOD(ConeBeamTestParams,
                 "Projected detector coordinate is computed for cone-parallel projection",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetParallel;
    using DetCol = DetFanLine;

    auto pg = std::vector<ProjGeom<DetRow, DetCol>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow, DetCol>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Geom::Utils::wrapAngle(pg.at(proj_idx).rot_ang.val) == Approx{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f},
                                              DetCol{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float3(0.0f, 0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(),
                                           proj_idx, backproj_weight);

        REQUIRE(det_coord.x == Approx{0.0f});
        REQUIRE(det_coord.y == Approx{0.0f});
        REQUIRE(backproj_weight == Approx{1.0f});
    }

    SECTION("The sign of detector coordinate is correctly computed") {
        auto backproj_weight = 0.0f;
        auto const pix_pos_pos = make_float3(2.0f, 0.0f, 2.0f);
        auto const det_coord_pos = projMapping(pix_pos_pos, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);

        auto const pix_pos_neg = make_float3(-2.0f, 0.0f, -2.0f);
        auto const det_coord_neg = projMapping(pix_pos_neg, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);
        REQUIRE(det_coord_pos.x > det_coord_neg.x);
        REQUIRE(det_coord_pos.y < det_coord_neg.y);
        REQUIRE(std::fabs(det_coord_pos.x) == Approx{std::fabs(det_coord_neg.x)});
        REQUIRE(std::fabs(det_coord_pos.y) == Approx{std::fabs(det_coord_neg.y)});
    }
}

TEST_CASE_METHOD(ConeBeamTestParams,
                 "Projected detector coordinate is computed for cone-beam projection (cylindrical detector)",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetFanArc;
    using DetCol = DetFanLine;

    auto pg = std::vector<ProjGeom<DetRow, DetCol>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow, DetCol>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Detail::rotAng(pg.at(proj_idx).src_pos) == Units::Radians{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f},
                                              DetCol{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float3(0.0f, 0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(),
                                           proj_idx, backproj_weight);

        REQUIRE(det_coord.x == Approx{0.0f});
        REQUIRE(det_coord.y == Approx{0.0f});
        REQUIRE(backproj_weight == Approx{0.002f});
    }

    SECTION("The sign of detector coordinate is correctly computed") {
        auto backproj_weight = 0.0f;
        auto const pix_pos_pos = make_float3(2.0f, 0.0f, 2.0f);
        auto const det_coord_pos = projMapping(pix_pos_pos, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);

        auto const pix_pos_neg = make_float3(-2.0f, 0.0f, -2.0f);
        auto const det_coord_neg = projMapping(pix_pos_neg, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);
        REQUIRE(det_coord_pos.x > det_coord_neg.x);
        REQUIRE(det_coord_pos.y < det_coord_neg.y);
        REQUIRE(std::fabs(det_coord_pos.x) == Approx{std::fabs(det_coord_neg.x)});
        REQUIRE(std::fabs(det_coord_pos.y) == Approx{std::fabs(det_coord_neg.y)});
    }
}

TEST_CASE_METHOD(ConeBeamTestParams,
                 "Projected detector coordinate is computed for cone-beam projection (flat detector)",
                 "[geom][coord_trans_proj]")
{
    using namespace Geom;
    using namespace Geom::CUDA;
    using DetRow = DetFanLine;
    using DetCol = DetFanLine;

    auto pg = std::vector<ProjGeom<DetRow, DetCol>>{};
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(ProjGeomFactory<DetRow, DetCol>::create(params, proj_idx));
    }
    auto constexpr proj_idx = 2u;
    REQUIRE(Detail::rotAng(pg.at(proj_idx).src_pos) == Units::Radians{4.7123889923f});

    auto constexpr det_geom = std::make_tuple(DetRow{512u, 1.0f},
                                              DetCol{512u, 1.0f});
    auto const cg_dev = GCT::Geom::CUDA::CTGeomDev{CTGeom{det_geom, pg}};
    REQUIRE(cg_dev.projGeom());
    auto const pg_dev = cg_dev.projGeom();
    REQUIRE(pg_dev);

    SECTION("World origin is mapped to 0.0f") {
        auto const pix_pos_wcs = make_float3(0.0f, 0.0f, 0.0f);
        auto backproj_weight = 0.0f;
        auto const det_coord = projMapping(pix_pos_wcs, cg_dev.projGeom(),
                                           proj_idx, backproj_weight);

        REQUIRE(det_coord.x == Approx{0.0f});
        REQUIRE(det_coord.y == Approx{0.0f});
        REQUIRE(backproj_weight == Approx{2.0f});
    }

    SECTION("The sign of detector coordinate is correctly computed") {
        auto backproj_weight = 0.0f;
        auto const pix_pos_pos = make_float3(2.0f, 0.0f, 2.0f);
        auto const det_coord_pos = projMapping(pix_pos_pos, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);

        auto const pix_pos_neg = make_float3(-2.0f, 0.0f, -2.0f);
        auto const det_coord_neg = projMapping(pix_pos_neg, cg_dev.projGeom(),
                                               proj_idx, backproj_weight);
        REQUIRE(det_coord_pos.x > det_coord_neg.x);
        REQUIRE(det_coord_pos.y < det_coord_neg.y);
        REQUIRE(std::fabs(det_coord_pos.x) == Approx{std::fabs(det_coord_neg.x)});
        REQUIRE(std::fabs(det_coord_pos.y) == Approx{std::fabs(det_coord_neg.y)});
    }
}

}//GCT::Test namespace.



