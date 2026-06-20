#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/sampling_info.h"

namespace GCT::Test
{
TEST_CASE("1D sampler is initialized", "[geom][sampling_info_1d]")
{
    auto constexpr sampler = Geom::SamplingInfo1D{3u, 4.5f};

    REQUIRE(sampler.num_samples == 3u);
    REQUIRE(sampler.sampling_width == Approx{4.5f});
}

}//GCT::Geom::Test namespace.