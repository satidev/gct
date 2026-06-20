#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/recon_img_geom.h"
#include "../../../src/cpp/data/recon_img.h"
#include "../../../src/cpp/data/proj_meta_info.h"

namespace GCT::Test
{
TEST_CASE("2D image reconstruction from 1D projections when detector column info is absent",
          "[geom][img_dim]")
{
    REQUIRE(1u == Data::projDim<>());
    REQUIRE(2u == Data::reconImgDim<>());
}

TEST_CASE("3D image reconstruction from 2D projections when detector column info is present",
          "[geom][img_dim]")
{
    REQUIRE(2u == Data::projDim<Geom::DetFanArc>());
    REQUIRE(3u == Data::reconImgDim<Geom::DetFanArc>());
}

}//GCT::Test namespace.


