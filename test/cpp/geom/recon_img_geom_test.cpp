#include "../../../thirdparty/catch.hpp"
#include "recon_img_geom_test_fix.h"

namespace GCT::Test
{
TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Geometry of 2D recon image is initialized",
                 "[geom][recon_img_geom]")
{
    REQUIRE(geom2d.x.num_samples == 1u);
    REQUIRE(geom2d.x.sampling_width.val == Approx{2.3f});
    REQUIRE(geom2d.y.num_samples == 2u);
    REQUIRE(geom2d.y.sampling_width.val == Approx{3.4f});
    REQUIRE(geom2d.mid_point.x.val == Approx{1.2f});
    REQUIRE(geom2d.mid_point.y.val == Approx{3.5f});
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Geometry of 3D recon image is initialized",
                 "[geom][recon_img_geom]")
{
    REQUIRE(geom3d.x.num_samples == 1u);
    REQUIRE(geom3d.x.sampling_width.val == Approx{2.3f});
    REQUIRE(geom3d.y.num_samples == 2u);
    REQUIRE(geom3d.y.sampling_width.val == Approx{3.4f});
    REQUIRE(geom3d.z.num_samples == 3u);
    REQUIRE(geom3d.z.sampling_width.val == Approx{4.5f});
    REQUIRE(geom3d.mid_point.x.val == Approx{1.2f});
    REQUIRE(geom3d.mid_point.y.val == Approx{3.5f});
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Returns the image size in each dimension",
                 "[geom][recon_img_geom]")
{
    REQUIRE(Geom::reconImgSize(geom2d) == std::array{1u, 2u});
    REQUIRE(Geom::reconImgSize(geom3d) == std::array{1u, 2u, 3u});
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Returns true since the sampling is valid",
                 "[geom][recon_img_geom]")
{
    REQUIRE(Geom::isSamplingValid(geom2d));
    REQUIRE(Geom::isSamplingValid(geom2d));
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Returns false since the number of pixels is invalid",
                 "[geom][recon_img_geom]")
{
    auto invalid_1 = geom2d;
    invalid_1.x.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_1));

    auto invalid_2 = geom2d;
    invalid_2.y.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_2));

    auto invalid_3 = geom2d;
    invalid_3.x.num_samples = 0.0f;
    invalid_3.y.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_3));
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Returns false since the number of voxels is invalid",
                 "[geom][recon_img_geom]")
{
    auto invalid_1 = geom3d;
    invalid_1.x.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_1));

    auto invalid_2 = geom3d;
    invalid_2.y.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_2));

    auto invalid_3 = geom3d;
    invalid_3.z.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_3));

    auto invalid_4 = geom3d;
    invalid_4.x.num_samples = 0.0f;
    invalid_4.y.num_samples = 0.0f;
    invalid_4.z.num_samples = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_4));
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Returns false due to the invalid pixel size",
                 "[geom][recon_img_geom]")
{
    auto invalid_1 = geom2d;
    invalid_1.x.sampling_width.val = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_1));

    auto invalid_2 = geom2d;
    invalid_2.y.sampling_width.val = -1.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_2));

    auto invalid_3 = geom2d;
    invalid_3.x.sampling_width.val = -4.0f;
    invalid_3.y.sampling_width.val = -2.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_3));
}

TEST_CASE_METHOD(ReconImgGeomTestFix,
                 "Returns false due to the invalid voxel size",
                 "[geom][recon_img_geom]")
{
    auto invalid_1 = geom3d;
    invalid_1.x.sampling_width.val = 0.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_1));

    auto invalid_2 = geom3d;
    invalid_2.y.sampling_width.val = -10.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_2));

    auto invalid_3 = geom3d;
    invalid_3.z.sampling_width.val = -80.0f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_3));

    auto invalid_4 = geom3d;
    invalid_4.x.sampling_width.val = 0.0f;
    invalid_4.y.sampling_width.val = -20.0f;
    invalid_4.z.sampling_width.val = -0.7f;
    REQUIRE_FALSE(Geom::isSamplingValid(invalid_4));
}

TEST_CASE("Returns -127.5 as the left end position of the number line",
          "[geom][recon_img_geom]")
{
    REQUIRE(Geom::leftEndPointPos(0.0f, 256.0, 1.0f) == Approx{-127.5});
}

TEST_CASE("Returns the first pixel position", "[geom][recon_img_geom]")
{
    auto constexpr geom = Geom::ReconImgGeom<2u>{Geom::EqDistSampler {1025u, 1.0f},
                                                 Geom::EqDistSampler{513u, 2.0f},
                                                 Geom::VecWCS<2u>{1.0f, 2.0f}};
    auto constexpr pos = Geom::firstVoxPos(geom);
    REQUIRE(pos.x.val == Approx{-511.0f});
    REQUIRE(pos.y.val == Approx{-510.0f});
}

TEST_CASE("Returns the first voxel position", "[geom][recon_img_geom]")
{
    auto constexpr geom = Geom::ReconImgGeom<3u>{Geom::EqDistSampler{1025u, 1.0f},
                                                 Geom::EqDistSampler{513u, 2.0f},
                                                 Geom::EqDistSampler{11u, 4.0f},
                                                 Geom::VecWCS<3u>{1.0f, 2.0f, 3.0f}};
    auto constexpr pos = Geom::firstVoxPos(geom);
    REQUIRE(pos.x.val == Approx{-511.0f});
    REQUIRE(pos.y.val == Approx{-510.0f});
    REQUIRE(pos.z.val == Approx{-17.0f});
}

}//GCT::Test namespace.