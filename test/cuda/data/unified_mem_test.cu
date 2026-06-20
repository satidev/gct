#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/unified_mem.cuh"
#include "../../../src/cpp/geom/detector_1d.h"

namespace GCT::Test
{
TEMPLATE_TEST_CASE("An array of numbers is allocated in unified memory",
                   "[data][unified_mem]",
                   float, int, double, Geom::DetParallel)
{
    auto constexpr num_elem = 10u;
    auto const alloc = Data::CUDA::UnifiedMem<TestType>{num_elem};
    REQUIRE(alloc.numElements() == num_elem);
    REQUIRE(alloc.dataPointer());
}

TEST_CASE("Exception is thrown since the number of elements is invalid (unified memory)",
          "[data][host_mem_pinned]")
{
    auto constexpr num_elem = 0u;
    REQUIRE_THROWS_AS(Data::CUDA::UnifiedMem<float>{num_elem},
                      std::invalid_argument);
}
}//GCT::Test namespace
