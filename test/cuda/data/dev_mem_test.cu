#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/dev_mem.cuh"
#include "../../../src/cpp/geom/detector_1d.h"

namespace GCT::Test
{
TEMPLATE_TEST_CASE("An array of numbers is allocated in device memory",
                   "[data][dev_mem]",
                   float, int, double, Geom::DetParallel)
{
    auto constexpr num_elem = 10u;
    auto const alloc = Data::CUDA::DevMem<TestType>{num_elem};
    REQUIRE(alloc.numElements() == num_elem);
    REQUIRE(alloc.dataPointer());
}

TEST_CASE("Exception is thrown since the number of elements is invalid (device memory)",
          "[data][dev_mem]")
{
    auto constexpr num_elem = 0u;
    REQUIRE_THROWS_AS(Data::CUDA::DevMem<float>{num_elem},
                      std::invalid_argument);
}
}//GCT::Test namespace

