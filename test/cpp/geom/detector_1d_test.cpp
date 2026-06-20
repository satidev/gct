#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/detector_1d.h"

namespace GCT::Test
{
TEST_CASE("DetParallel is initialized", "[geom][det_geom]")
{
    auto constexpr sampler = Geom::EqDistSampler {1u, 2.0f};
    auto constexpr det = Geom::DetParallel{sampler};

    REQUIRE(det.sampler.num_samples == 1u);
    REQUIRE(det.sampler.sampling_width == Geom::Units::MilliMeters{2.0f});
}

TEST_CASE("DetFanArc is initialized", "[geom][det_geom]")
{
    auto constexpr sampler = Geom::EqAngSampler {1u, 2.0f};
    auto constexpr det = Geom::DetFanArc{sampler};

    REQUIRE(det.sampler.num_samples == 1u);
    REQUIRE(det.sampler.sampling_width == Geom::Units::Radians{2.0f});
}

TEST_CASE("DetFanLine is initialized", "[geom][det_geom]")
{
    auto constexpr sampler = Geom::EqDistSampler {1u, 2.0f};
    auto constexpr det = Geom::DetFanLine{sampler};

    REQUIRE(det.sampler.num_samples == 1u);
    REQUIRE(det.sampler.sampling_width == Geom::Units::MilliMeters{2.0f});
}
}//GCT::Geom::Test namespace.

