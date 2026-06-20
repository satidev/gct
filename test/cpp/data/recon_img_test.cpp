#include "catch.hpp"
#include "../../../src/cpp/data/recon_img.h"
#include "../../../src/cpp/data/stat.h"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE("2D recon image is initialized",
          "[data][recon_img]")
{
    auto rg = Geom::ReconImgGeom<2u>{};
    rg.x.num_samples = 21u;
    rg.x.sampling_width.val = 0.56f;
    rg.y.num_samples = 23u;
    rg.y.sampling_width.val = 0.3f;
    rg.mid_point.x.val = 1.8f;
    rg.mid_point.y.val = 2.3f;

    auto const img = Data::ReconImg<float, 2u>{rg};

    REQUIRE(img.numElements() == rg.x.num_samples * rg.y.num_samples);
    REQUIRE(img.dimensions() == std::array{rg.x.num_samples, rg.y.num_samples});
    REQUIRE_THAT(GCT::Data::Stat::mean(img), AreEqualFloat(0.0f));

    REQUIRE(img.geom().x.num_samples == rg.x.num_samples);
    REQUIRE(img.geom().x.sampling_width == rg.x.sampling_width);
    REQUIRE(img.geom().y.num_samples == rg.y.num_samples);
    REQUIRE(img.geom().y.sampling_width == rg.y.sampling_width);

    REQUIRE(img.geom().mid_point.x == rg.mid_point.x);
    REQUIRE(img.geom().mid_point.y == rg.mid_point.y);
}

TEST_CASE("3D recon image is initialized",
          "[data][recon_img]")
{
    auto rg = Geom::ReconImgGeom<3u>{};
    rg.x.num_samples = 21u;
    rg.x.sampling_width.val = 0.56f;
    rg.y.num_samples = 23u;
    rg.y.sampling_width.val = 0.3f;
    rg.z.num_samples = 28u;
    rg.z.sampling_width.val = 0.9f;
    rg.mid_point.x.val = 1.8f;
    rg.mid_point.y.val = 2.3f;
    rg.mid_point.z.val = -8.3f;

    auto const img = Data::ReconImg<float, 3u>{rg};

    REQUIRE(img.numElements() == rg.x.num_samples * rg.y.num_samples * rg.z.num_samples);
    REQUIRE(img.dimensions() == std::array{rg.x.num_samples, rg.y.num_samples,
                                           rg.z.num_samples});
    REQUIRE_THAT(GCT::Data::Stat::mean(img), AreEqualFloat(0.0f));

    REQUIRE(img.geom().x.num_samples == rg.x.num_samples);
    REQUIRE(img.geom().x.sampling_width == rg.x.sampling_width);
    REQUIRE(img.geom().y.num_samples == rg.y.num_samples);
    REQUIRE(img.geom().y.sampling_width == rg.y.sampling_width);
    REQUIRE(img.geom().z.num_samples == rg.z.num_samples);
    REQUIRE(img.geom().z.sampling_width == rg.z.sampling_width);

    REQUIRE(img.geom().mid_point.x == rg.mid_point.x);
    REQUIRE(img.geom().mid_point.y == rg.mid_point.y);
    REQUIRE(img.geom().mid_point.z == rg.mid_point.z);
}

TEST_CASE("Exception is thrown if the sampling info is invalid (2D)",
          "[data][recon_img]")
{
    auto rg = Geom::ReconImgGeom<2u>{};
    rg.x.num_samples = 1u;
    rg.x.sampling_width.val = 0.3f;
    rg.y.num_samples = 2u;
    rg.y.sampling_width.val = 0.8f;
    REQUIRE_NOTHROW(Data::ReconImg<float, 2u>{rg});

    rg.x.sampling_width.val = 0.0f;
    REQUIRE_THROWS(Data::ReconImg<float, 2u>(rg));

    rg.x.sampling_width.val = 1.0f;
    rg.y.sampling_width.val = -1.0f;
    REQUIRE_THROWS(Data::ReconImg<float, 2u>{rg});
}

TEST_CASE("Exception is thrown if the sampling info is invalid (3D)",
          "[data][recon_img]")
{
    auto rg = Geom::ReconImgGeom<3u>{};
    rg.x.num_samples = 1u;
    rg.x.sampling_width.val = 0.3f;
    rg.y.num_samples = 2u;
    rg.y.sampling_width.val = 0.8f;
    rg.z.num_samples = 6u;
    rg.z.sampling_width.val = 6.5f;

    REQUIRE_NOTHROW(Data::ReconImg<float, 3u>{rg});

    rg.x.sampling_width.val = 0.0f;
    REQUIRE_THROWS(Data::ReconImg<float, 3u>{rg});

    rg.x.sampling_width.val = 1.0f;
    rg.y.sampling_width.val = -1.0f;
    REQUIRE_THROWS(Data::ReconImg<float, 3u>{rg});

    rg.x.sampling_width.val = 1.0f;
    rg.y.sampling_width.val = 1.0f;
    rg.z.sampling_width.val = 0.0f;
    REQUIRE_THROWS(Data::ReconImg<float, 3u>{rg});
}

}//GCT::Test namespace.