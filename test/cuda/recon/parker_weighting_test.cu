#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/recon/parker_weighting.cuh"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"
#include <filesystem>
#include "../../matchers.h"
#include "../../../src/cpp/io/parameters/proj_geom_params.h"
#include "../../../src/cpp/geom/axial_ct_geom.h"

namespace GCT::Test
{
using namespace GCT::Geom::Units;
using namespace GCT::Recon::CUDA;

template<typename DetRow>
constexpr DetRow detector();
template<>
constexpr Geom::DetFanArc detector()
{
    return Geom::DetFanArc{512u, 0.0017922f};
}
template<>
constexpr Geom::DetFanLine detector()
{
    return Geom::DetFanLine{512u, 2.0f};
}

template<typename DetRow>
Geom::CTGeom<DetRow> fanBeamGeom(Radians ang_range,
                                 Radians start_ang,
                                 Geom::RotDir dir)
{
    // Fan-beam projection geometry parameters for ideal circular trajectory.
    auto constexpr det_row = detector<Geom::DetFanArc>();
    auto const num_det_cols = det_row.sampler.num_samples;
    auto params = Geom::FanBeamProjGeomParams{};
    params.rot.init_ang = start_ang;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{ang_range.val / params.rot.num_proj};
    params.det_row_center = DetPixels{(static_cast<float>(num_det_cols) - 1.0f) / 2.0f};
    params.src_det_dist = 1000.0_mm;
    params.src_iso_dist = 500.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetFanArc>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg = Geom::ProjGeomFactory<Geom::DetFanArc>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Complete CT geometry (detector + projection geometries).
    return Geom::AxialCTGeom<Geom::DetFanArc>{std::make_tuple(det_row), proj_geom};
}

template<typename DetRow>
std::vector<float> parkerWeightsData(Geom::CTGeom<DetRow> const &geom)
{
    auto const weights_dev = getParkerWeights(geom);
    auto const num_det_cols = weights_dev.numCols();
    auto const num_proj = weights_dev.numRows();
    auto weights_host = std::vector<float>(num_det_cols * num_proj);
    GCT::Data::CUDA::copyDevToHost(weights_dev, weights_host);
    return weights_host;
}

TEST_CASE("The values of Parker weights are in [0.0, 1.0]",
          "[recon][parker_weight]")
{
    auto const start_ang = GENERATE(45.0_deg, 90.0_deg, 180.0_deg, 270.0_deg, 360.0_deg);
    auto const ang_range = GENERATE(200.0_deg, 250.0_deg, 360.0_deg);
    auto const dir = GENERATE(Geom::RotDir::CW, Geom::RotDir::CCW);
    auto const weights = parkerWeightsData(fanBeamGeom<Geom::DetFanArc>(ang_range, start_ang, dir));
    REQUIRE(std::all_of(std::begin(weights), std::end(weights),
                        [](float val)
                        {
                            return (val >= 0.0f) && (val <= 1.0f);
                        }
    ));
}

TEST_CASE("Parker weights are same irrespective of start angles",
          "[recon][parker_weight]")
{
    auto const weights_ref = parkerWeightsData(fanBeamGeom<Geom::DetFanArc>(360.0_deg, 0.0_deg, Geom::RotDir::CW));
    auto constexpr eps = 0.00001f;
    auto const start_ang = GENERATE(90.0_deg, 180.0_deg, 270.0_deg, 360.0_deg);
    auto const weights_test = parkerWeightsData(fanBeamGeom<Geom::DetFanArc>(360.0_deg, start_ang, Geom::RotDir::CW));
    REQUIRE_THAT(weights_ref, AreEqualFloatArrays(weights_test, eps));
}

TEST_CASE("Parker weights are different for different angular range",
          "[recon][parker_weight]")
{
    auto const weights_ref = parkerWeightsData(fanBeamGeom<Geom::DetFanArc>(255.0_deg, 90.0_deg, Geom::RotDir::CW));
    auto const ang_range = GENERATE(90.0_deg, 180.0_deg, 270.0_deg, 360.0_deg);
    auto const weights_test = parkerWeightsData(fanBeamGeom<Geom::DetFanArc>(ang_range, 90.0_deg, Geom::RotDir::CW));
    REQUIRE_FALSE(weights_ref == weights_test);
}
} //GCT::Test namespace.
