#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/copy_host_dev.cuh"
#include "../../../src/cuda/data/stat.cuh"
#include "../../../src/cuda/data/projection.cuh"
#include <vector>
#include "../../matchers.h"
#include "nd_array_dev_test_fix.cuh"

namespace GCT::Test
{

TEMPLATE_TEST_CASE_SIG(
    "The elements of STL vector are copied to CUDA device and then copied back to another STL vector",
    "[data][copy_host_dev]",
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
    auto constexpr init_val = -6.0;
    auto const vec = std::vector(arr.numElements(), static_cast<type>(init_val));

    copyHostToDev(vec, arr);
    auto vec_op = std::vector(arr.numElements(), type{});
    copyDevToHost(arr, vec_op);
    REQUIRE(vec_op == vec);
}

TEST_CASE("Single projection of the stack is copied to CUDA device",
          "[data][copy_host_dev]")
{
    auto constexpr proj_dim = 1u;
    auto constexpr num_cols = 3u;
    auto constexpr proj_size = Data::ProjSize<proj_dim>{num_cols};
    auto constexpr num_proj = 2u;
    auto proj_host = GCT::Data::ProjectionStack<float, proj_dim>{proj_size, num_proj};
    proj_host.setData(std::vector<float>{1.0f, 1.0f, 1.0f,//first row
                                         2.0f, 2.0f, 2.0f//second row
                      }
    );

    typedef Data::CUDA::DevMem<float> DevArray;
    auto proj_dev = Data::CUDA::Projection<float, proj_dim, DevArray>{proj_size};
    GCT::Data::CUDA::copyHostToDev(proj_host.elements(),
                                   proj_dev,
                                   0u,
                                   0u,
                                   proj_dev.numElements());//Copy the first projection.
    REQUIRE(Data::Stat::CUDA::mean(proj_dev) == 1.0f);
    Data::CUDA::copyHostToDev(proj_host.elements(), proj_dev, proj_dev.numElements(), 0u,
                              proj_dev.numElements());//Copy the second projection.
    REQUIRE(Data::Stat::CUDA::mean(proj_dev) == 2.0f);
    REQUIRE_THROWS_AS(Data::CUDA::copyHostToDev(proj_host.elements(), proj_dev,
                                                2 * proj_dev.numElements(), 0u,
                                                proj_dev.numElements()),
                      std::runtime_error);
}

TEST_CASE("A collection of objects is copied to unified memory",
          "[data][copy_host_dev]")
{
    auto coll = std::vector<Geom::DetParallel>{};
    coll.push_back(Geom::DetParallel{23u, 0.67f});
    coll.push_back(Geom::DetParallel{25u, 1.37f});
    coll.push_back(Geom::DetParallel{27u, 4.67f});

    auto alloc = Data::CUDA::UnifiedMem<GCT::Geom::DetParallel>{static_cast<uint>(coll.size())};
    REQUIRE(alloc.numElements() == coll.size());

    Data::CUDA::copyHostToDev(coll, alloc);
    REQUIRE(alloc.dataPointer()[0].sampler.num_samples == 23u);
    REQUIRE(alloc.dataPointer()[2].sampler.sampling_width.val == Approx{4.67f});
}

}//GCT::Test namespace.
