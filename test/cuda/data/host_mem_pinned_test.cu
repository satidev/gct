#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/host_mem_pinned.cuh"
#include "../../../src/cpp/geom/detector_1d.h"

namespace GCT::Test
{
TEMPLATE_TEST_CASE("An array of numbers is allocated in host pinned memory",
                   "[data][host_mem_pinned]",
                   float, int, double, Geom::DetParallel)
{
    auto constexpr num_elem = 10u;
    auto const alloc = Data::CUDA::HostMemPinned<TestType>{num_elem};
    REQUIRE(alloc.numElements() == num_elem);
    REQUIRE(alloc.dataPointer());
}

TEST_CASE("Exception is thrown since the number of elements is invalid (host pinned memory)",
          "[data][host_mem_pinned]")
{
    auto constexpr num_elem = 0u;
    REQUIRE_THROWS_AS(Data::CUDA::HostMemPinned<float>{num_elem},
                      std::invalid_argument);
}
}//GCT::Test namespace


