#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/stat.cuh"
#include "../../../src/cuda/data/vector_dev.cuh"

namespace GCT::Test
{
TEST_CASE("Sum and mean of element values are computed correctly",
          "[data][stat]")
{
    auto constexpr num_elem = 23u;
    auto vec = Data::CUDA::VectorDev<float, Data::CUDA::DevMem<float>>{num_elem};
    auto constexpr elem_val = 3.0f;
    vec.setVal(elem_val);

    REQUIRE(GCT::Data::Stat::CUDA::sum(vec) == num_elem*elem_val);
    REQUIRE(GCT::Data::Stat::CUDA::mean(vec) == elem_val);
}

}
