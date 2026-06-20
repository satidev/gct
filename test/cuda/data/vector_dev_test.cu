#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/vector_dev.cuh"

namespace GCT::Test
{
TEST_CASE("Vector in CUDA device is initialized", "[vector_dev_test]")
{
    auto constexpr num_elem = 23u;
    auto vec = Data::CUDA::VectorDev<float, GCT::Data::CUDA::DevMem<float>>{num_elem};
    REQUIRE(vec.numElements() == num_elem);
    REQUIRE(vec.dimensions() == std::array{num_elem});
}

}//GCT::Test namespace.

