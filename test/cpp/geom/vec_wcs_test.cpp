#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/vec_wcs.h"

namespace GCT::Test
{
TEST_CASE("2D vector in world coordinate system is initialized",
          "[geom][vec_wcs]")
{
    auto constexpr vec = Geom::VecWCS<2u>{3.0f, 4.5f};
    REQUIRE(vec.x.val == Approx{3.0f});
    REQUIRE(vec.y.val == Approx{4.5f});
}

TEST_CASE("3D vector in world coordinate system is initialized",
          "[geom][vec_wcs]")
{
    auto constexpr vec = Geom::VecWCS<3u>{3.0f, 4.5f, -8.1f};
    REQUIRE(vec.x.val == Approx{3.0f});
    REQUIRE(vec.y.val == Approx{4.5f});
    REQUIRE(vec.z.val == Approx{-8.1f});
}

}//GCT::Geom::Test namespace.
