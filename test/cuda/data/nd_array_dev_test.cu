#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/nd_array_dev.cuh"
#include "../../../src/cuda/data/unified_mem.cuh"
#include <cuComplex.h>
#include "../../../src/cuda/data/copy_host_dev.cuh"
#include "nd_array_dev_test_fix.cuh"

namespace GCT::Test
{
TEMPLATE_TEST_CASE_SIG(
    "N-D array is allocated in CUDA device",
    "[data][nd_array_dev]",
    ((typename DevMemType, size_t Dim), DevMemType, Dim),
    (Data::CUDA::DevMem<int>, 1u),
    (Data::CUDA::DevMem<float>, 2u),
    (Data::CUDA::DevMem<double>, 3u),
    (Data::CUDA::UnifiedMem<int>, 1u),
    (Data::CUDA::UnifiedMem<float>, 2u),
    (Data::CUDA::UnifiedMem<double>, 3u))

{
    typedef typename DevMemType::value_type type;
    auto constexpr dims = testArrSize<Dim>();
    auto arr = Data::CUDA::NDArrayDev<type, dims.size(), DevMemType>{dims};
    REQUIRE(arr.dataPointer());
    REQUIRE(arr.numElements() == std::accumulate(std::cbegin(dims),
                                                 std::cend(dims),
                                                 1u, std::multiplies()));
    REQUIRE(arr.dimensions() == dims);
}

TEST_CASE("Exception is thrown if the array dimension is invalid",
          "[data][nd_array_dev]")
{
    auto constexpr valid_dim = std::array{5u};
    REQUIRE_NOTHROW(
        Data::CUDA::NDArrayDev<float, valid_dim.size(), Data::CUDA::DevMem<float>>{valid_dim});

    auto constexpr invalid_dim = std::array{5u, 0u};
    REQUIRE_THROWS(
        Data::CUDA::NDArrayDev<float, invalid_dim.size(), Data::CUDA::DevMem<float>>{invalid_dim});
}

TEMPLATE_TEST_CASE("Element values are set to 4.0",
                   "[data][nd_array_dev]",
                   Data::CUDA::DevMem<float>,
                   Data::CUDA::UnifiedMem<float>)
{
    auto constexpr dims = std::array{2u, 3u, 4u};
    auto arr = Data::CUDA::NDArrayDev<float, dims.size(), TestType>{dims};
    auto constexpr val = 4.0f;
    arr.setVal(val);

    auto vec = std::vector(arr.numElements(), 0.0f);
    copyDevToHost(arr, vec);
    auto exp_op = std::vector(arr.numElements(), val);
    REQUIRE(vec == exp_op);
}

TEMPLATE_TEST_CASE("Element values are set to (2+i3)",
                   "[data][nd_array_dev]",
                   Data::CUDA::DevMem<cuComplex>,
                   Data::CUDA::UnifiedMem<cuComplex>)
{
    auto constexpr dims = std::array{5u, 4u};
    auto arr = Data::CUDA::NDArrayDev<cuComplex, dims.size(), TestType>{dims};
    auto const val = cuComplex{2.0f, 3.0f};
    arr.setVal(val);

    auto vec = std::vector<cuComplex>(arr.numElements());
    copyDevToHost(arr, vec);
    auto const success = std::all_of(std::cbegin(vec), std::cend(vec), [val](cuComplex const &elem)
                                     {
                                         return (elem.x == val.x) && (elem.y == val.y);
                                     }
    );
    REQUIRE(success);
}

TEMPLATE_TEST_CASE("Scalar multiplication (3*2 == 6)",
                   "[data][nd_array_dev]",
                   Data::CUDA::DevMem<float>,
                   Data::CUDA::UnifiedMem<float>)
{
    auto constexpr dims = std::array{5u, 4u};
    auto arr = Data::CUDA::NDArrayDev<float, dims.size(), TestType>{dims};
    arr.setVal(3.0f);
    arr *= 2.0f;

    auto vec = std::vector<float>(arr.numElements());
    copyDevToHost(arr, vec);
    auto const exp_op = std::vector(arr.numElements(), 6.0f);
    REQUIRE(vec == exp_op);
}

TEMPLATE_TEST_CASE("Element-wise addition (3+2 == 5)",
                   "[data][nd_array_dev]",
                   Data::CUDA::DevMem<float>,
                   Data::CUDA::UnifiedMem<float>)
{
    auto constexpr dims = std::array{5u, 4u};
    auto arr_1 = Data::CUDA::NDArrayDev<float, dims.size(), TestType>{dims};
    arr_1.setVal(3.0f);
    auto arr_2 = Data::CUDA::NDArrayDev<float, dims.size(), TestType>{dims};
    arr_2.setVal(2.0f);

    arr_1 += arr_2;

    auto vec = std::vector<float>(arr_1.numElements());
    copyDevToHost(arr_1, vec);
    auto const exp_op = std::vector(arr_1.numElements(), 5.0f);
    REQUIRE(vec == exp_op);
}

}//GCT::Test namespace.
