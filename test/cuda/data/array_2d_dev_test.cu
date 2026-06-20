#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/array_2d_dev.cuh"

namespace GCT::Test
{
TEST_CASE("2D array in CUDA device is allocated and initialized",
          "[data][array_2d_dev]")
{
    auto constexpr num_cols = 23u;
    auto constexpr num_rows = 11u;
    using namespace GCT::Data::CUDA;
    auto const arr = Array2DDev<float, DevMem<float>>{num_cols, num_rows};

    REQUIRE(arr.numElements() == num_cols * num_rows);
    REQUIRE(arr.dimensions() == std::array{num_cols, num_rows});
    REQUIRE(arr.numCols() == num_cols);
    REQUIRE(arr.numRows() == num_rows);
    REQUIRE(arr.dataPointer());
}

}//GCT::Test namespace.

