#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/geom/coord_trans_img.cuh"

namespace GCT::Test
{
TEST_CASE("Returns the left end position of the number line",
          "[geom][coord_trans_img]")
{
    auto constexpr mid_point = 0.0f;
    auto constexpr num_samples = 256u;
    auto constexpr sampling_width = 1.0f;

    REQUIRE(Geom::CUDA::leftEndPointPos(mid_point, num_samples, sampling_width) ==
        -127.5f);
}

TEST_CASE("Returns the first pixel position", "[geom][coord_trans_img]")
{
    auto constexpr geom = Geom::ReconImgGeom<2u>{Geom::EqDistSampler{1025u, 1.0f},
                                                 Geom::EqDistSampler{513u, 2.0f},
                                                 Geom::VecWCS<2u>{1.0f, 2.0f}};
    auto const pos = Geom::CUDA::firstPixPos(&geom);
    REQUIRE(pos.x == -511.0f);
    REQUIRE(pos.y == -510.0f);
}

TEST_CASE("Returns the first voxel position", "[geom][coord_trans_img]")
{
    auto constexpr geom = Geom::ReconImgGeom<3u>{Geom::EqDistSampler{1025u, 1.0f},
                                                 Geom::EqDistSampler{513u, 2.0f},
                                                 Geom::EqDistSampler{11u, 4.0f},
                                                 Geom::VecWCS<3u>{1.0f, 2.0f, 3.0f}};
    auto const pos = Geom::CUDA::firstVoxPos(&geom);
    REQUIRE(pos.x == Approx{-511.0f});
    REQUIRE(pos.y == Approx{-510.0f});
    REQUIRE(pos.z == Approx{-17.0f});
}

TEST_CASE("Returns the affine function value", "[geom][coord_trans_img]")
{
    auto constexpr translation = -127.5f;
    auto constexpr scale = 1.0f;
    REQUIRE(Geom::CUDA::affineFunc(translation, 0, scale) == -127.5f);
}

TEST_CASE("Returns the position vector of pixel in WCS", "[geom][coord_trans_img]")
{
    auto constexpr geom = Geom::ReconImgGeom<2u>{Geom::EqDistSampler{512u, 2.0f},
                                                 Geom::EqDistSampler{256u, 1.0f},
                                                 Geom::VecWCS<2u>{0.0f, 0.0f}};
    auto const pos = Geom::CUDA::pixPosWCS(&geom, uint2{0u, 0u});
    REQUIRE(pos.x == -511.0f);
    REQUIRE(pos.y == -127.5f);
}

TEST_CASE("Returns the position vector of voxel in WCS", "[geom][coord_trans_img]")
{
    auto constexpr geom = Geom::ReconImgGeom<3u>{Geom::EqDistSampler{256u, 1.0f},
                                                 Geom::EqDistSampler{256u, 1.0f},
                                                 Geom::EqDistSampler{256u, 1.0f},
                                                 Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}};
    auto const pos = Geom::CUDA::voxPosWCS(&geom, uint3{0u, 0u, 0u});
    REQUIRE(pos.x == -127.5f);
    REQUIRE(pos.y == -127.5f);
    REQUIRE(pos.z == -127.5f);
}

}//GCT::Test namespace.


