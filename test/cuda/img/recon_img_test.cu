#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/data/recon_img.cuh"
#include "../../../src/cuda/data/stat.cuh"
#include "../../../src/cpp/data/stat.h"

using namespace GCT;
using namespace GCT::Geom;

TEST_CASE("2D recon image is initialized in CUDA device", "[recon_img_cu]")
{
    Geom::ReconImgGeom<2u> rg{};
    rg.x.num_samples = 21u;
    rg.x.sampling_width.val = 0.56f;
    rg.y.num_samples = 23u;
    rg.y.sampling_width.val = 0.3f;
    rg.mid_point.x.val = 1.8f;
    rg.mid_point.y.val = 2.3f;

    typedef Data::CUDA::UnifiedMem<float> Alloc;
    Data::CUDA::ReconImg<float, 2u, Alloc> img{rg};

    REQUIRE(img.numElements() == rg.x.num_samples * rg.y.num_samples);
    REQUIRE(img.dimensions() == std::array<unsigned int, 2u>{rg.x.num_samples, rg.y.num_samples});
    REQUIRE(Data::Stat::CUDA::mean(img) == Approx{0.0});

    REQUIRE(img.geom()->x.num_samples == rg.x.num_samples);
    REQUIRE(img.geom()->x.sampling_width == rg.x.sampling_width);
    REQUIRE(img.geom()->y.num_samples == rg.y.num_samples);
    REQUIRE(img.geom()->y.sampling_width == rg.y.sampling_width);

    REQUIRE(img.geom()->mid_point.x == rg.mid_point.x);
    REQUIRE(img.geom()->mid_point.y == rg.mid_point.y);
}

TEST_CASE("3D recon image is initialized in CUDA device", "[recon_img_cu]")
{
    Geom::ReconImgGeom<3u> rg{};
    rg.x.num_samples = 21u;
    rg.x.sampling_width.val = 0.56f;
    rg.y.num_samples = 23u;
    rg.y.sampling_width.val = 0.3f;
    rg.z.num_samples = 28u;
    rg.z.sampling_width.val = 0.9f;
    rg.mid_point.x.val = 1.8f;
    rg.mid_point.y.val = 2.3f;
    rg.mid_point.z.val = -8.3f;

    typedef Data::CUDA::UnifiedMem<float> Alloc;
    Data::CUDA::ReconImg<float, 3u, Alloc> img{rg};

    REQUIRE(img.numElements() == rg.x.num_samples * rg.y.num_samples * rg.z.num_samples);
    REQUIRE(img.dimensions() == std::array<unsigned int, 3u>{rg.x.num_samples, rg.y.num_samples,
                                                       rg.z.num_samples});
    REQUIRE(GCT::Data::Stat::CUDA::mean(img) == Approx{0.0});

    REQUIRE(img.geom()->x.num_samples == rg.x.num_samples);
    REQUIRE(img.geom()->x.sampling_width == rg.x.sampling_width);
    REQUIRE(img.geom()->y.num_samples == rg.y.num_samples);
    REQUIRE(img.geom()->y.sampling_width == rg.y.sampling_width);
    REQUIRE(img.geom()->z.num_samples == rg.z.num_samples);
    REQUIRE(img.geom()->z.sampling_width == rg.z.sampling_width);

    REQUIRE(img.geom()->mid_point.x == rg.mid_point.x);
    REQUIRE(img.geom()->mid_point.y == rg.mid_point.y);
    REQUIRE(img.geom()->mid_point.z == rg.mid_point.z);
}

TEST_CASE("3D recon image is copied from device to host", "[recon_img_cu]")
{
    Geom::ReconImgGeom<3u> rg{};
    rg.x.num_samples = 21u;
    rg.x.sampling_width.val = 0.56f;
    rg.y.num_samples = 23u;
    rg.y.sampling_width.val = 0.3f;
    rg.z.num_samples = 28u;
    rg.z.sampling_width.val = 0.9f;
    rg.mid_point.x.val = 1.8f;
    rg.mid_point.y.val = 2.3f;
    rg.mid_point.z.val = -8.3f;

    typedef Data::CUDA::UnifiedMem<float> Alloc;
    Data::CUDA::ReconImg<float, 3u, Alloc> dev{rg};
    dev.setVal(6.0f);
    REQUIRE(GCT::Data::Stat::CUDA::mean(dev) == Approx{6.0});

    Data::ReconImg<float, 3u> host{rg};
    std::vector<float> vec(host.numElements());
    Data::CUDA::copyDevToHost(dev, vec);
    host.setData(std::move(vec));

    REQUIRE(host.dimensions() == dev.dimensions());
    REQUIRE(Data::Stat::mean(host) == Approx{6.0});
}

TEST_CASE("Exception is thrown for invalid sampling info (2D)", "[recon_img]")
{
    ReconImgGeom<2u> rg{};
    rg.x.num_samples = 1u;
    rg.x.sampling_width.val = 0.3f;
    rg.y.num_samples = 2u;
    rg.y.sampling_width.val = 0.8f;

    REQUIRE_NOTHROW(Data::CUDA::ReconImg<float, 2u, GCT::Data::CUDA::UnifiedMem<float>>{rg});

    rg.x.sampling_width.val = 0.0f;
    REQUIRE_THROWS(Data::CUDA::ReconImg<float, 2u, GCT::Data::CUDA::UnifiedMem<float>>{rg});

    rg.x.sampling_width.val = 1.0f;
    rg.y.sampling_width.val = -1.0f;
    REQUIRE_THROWS(Data::CUDA::ReconImg<float, 2u, GCT::Data::CUDA::UnifiedMem<float>>{rg});
}

TEST_CASE("Exception is thrown for invalid sampling info (3D)", "[recon_img]")
{
    ReconImgGeom<3u> rg{};
    rg.x.num_samples = 1u;
    rg.x.sampling_width.val = 0.3f;
    rg.y.num_samples = 2u;
    rg.y.sampling_width.val = 0.8f;
    rg.z.num_samples = 6u;
    rg.z.sampling_width.val = 6.5f;

    REQUIRE_NOTHROW(Data::CUDA::ReconImg<float, 3u, GCT::Data::CUDA::UnifiedMem<float>>{rg});

    rg.x.sampling_width.val = 0.0f;
    REQUIRE_THROWS(Data::CUDA::ReconImg<float, 3u, GCT::Data::CUDA::UnifiedMem<float>>{rg});

    rg.x.sampling_width.val = 1.0f;
    rg.y.sampling_width.val = -1.0f;
    REQUIRE_THROWS(Data::CUDA::ReconImg<float, 3u, GCT::Data::CUDA::UnifiedMem<float>>{rg});

    rg.x.sampling_width.val = 1.0f;
    rg.y.sampling_width.val = 1.0f;
    rg.z.sampling_width.val = 0.0f;
    REQUIRE_THROWS(Data::CUDA::ReconImg<float, 3u, GCT::Data::CUDA::UnifiedMem<float>>{rg});
}
