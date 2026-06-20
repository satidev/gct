#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/projection.cuh"
#include "../../../src/cuda/data/stat.cuh"
#include "../../../src/cuda/data/dev_mem_texture.cuh"

TEST_CASE("2D projection in CUDA device (unified mem) is initialized", "[proj_img]")
{
    GCT::Data::ProjSize<2u> size{2u, 4u};
    typedef GCT::Data::CUDA::UnifiedMem<float> Alloc;
    GCT::Data::CUDA::Projection<float, 2u, Alloc> proj{size};
    REQUIRE(proj.numElements() == 8u);
    REQUIRE(proj.projSize().num_cols == 2u);
    REQUIRE(proj.projSize().num_rows == 4u);

    proj.setVal(1.0f);
    proj *= 4.0f;
    REQUIRE(GCT::Data::Stat::CUDA::mean(proj) == Approx{4.0f});
}



