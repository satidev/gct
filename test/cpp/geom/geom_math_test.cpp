#include "catch.hpp"
#include "../../../src/cpp/geom/geom_math.h"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE("Norm of the vector in 2D WCS is computed",
          "[geom][geom_math]")
{
    SECTION("Returns 5.0") {
        auto constexpr vec = Geom::VecWCS<2u>{-3.0f, 4.0f};
        REQUIRE_THAT(Geom::Math::norm(vec), AreEqualFloat(5.0f));
    }
}

TEST_CASE("Norm of the vector in 3D WCS is computed",
          "[geom][geom_math]")
{
    SECTION("Returns 13.0") {
        auto constexpr vec = Geom::VecWCS<3u>{3.0f, -4.0f, 12.0f};
        REQUIRE_THAT(Geom::Math::norm(vec), AreEqualFloat(13.0f));
    }
}

TEST_CASE("2D vector after 90 rotation is computed",
          "[geom][geom_math]")
{
    SECTION("Returns y-axis") {
        auto constexpr vec = Geom::VecWCS<2u>{1.0f, 0.0f};
        auto constexpr rot_vec = Geom::Math::rot90(vec);
        REQUIRE_THAT(rot_vec, AreEqualVecWCS(Geom::VecWCS<2u>{0.0f, 1.0f}));
    }

    SECTION("Returns negative x-axis") {
        auto constexpr vec = Geom::VecWCS<2u>{0.0f, 1.0f};
        auto constexpr rot_vec = Geom::Math::rot90(vec);
        REQUIRE_THAT(rot_vec, AreEqualVecWCS(Geom::VecWCS<2u>{-1.0f, 0.0f}));
    }
}

TEST_CASE("Cross product of two 3D vector is computed",
          "[geom][geom_math]")
{
    SECTION("Returns 0 vector") {
        auto constexpr v1 = Geom::VecWCS<3u>{0.0f, 0.0f, 1.0f};
        auto constexpr v2 = Geom::VecWCS<3u>{0.0f, 0.0f, 1.0f};
        auto const cross = Geom::Math::crossProduct(v1, v2);
        REQUIRE_THAT(cross, AreEqualVecWCS(Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}));
    }
    SECTION("Returns z-axis") {
        auto constexpr v1 = Geom::VecWCS<3u>{1.0f, 0.0f, 0.0f};
        auto constexpr v2 = Geom::VecWCS<3u>{0.0f, 1.0f, 0.0f};
        auto const cross = Geom::Math::crossProduct(v1, v2);
        REQUIRE_THAT(cross, AreEqualVecWCS(Geom::VecWCS<3u>{0.0f, 0.0f, 1.0f}));
    }

    SECTION("Returns y-axis") {
        auto constexpr v1 = Geom::VecWCS<3u>{1.0f, 0.0f, 0.0f};
        auto constexpr v2 = Geom::VecWCS<3u>{0.0f, 0.0f, -1.0f};
        auto const cross = Geom::Math::crossProduct(v1, v2);
        REQUIRE_THAT(cross, AreEqualVecWCS(Geom::VecWCS<3u>{0.0f, 1.0f, 0.0f}));
    }

}

}//GCT::Test namespace.