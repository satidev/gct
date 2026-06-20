#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/geom/vec_wcs_math.cuh"
#include "../../matchers.h"

namespace GCT::Test
{
TEST_CASE("Returns the result of vector addition", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec_1 = Geom::VecWCS<2u>{1.0f, 2.0f};
        auto constexpr vec_2 = Geom::VecWCS<2u>{3.0f, 4.0f};
        auto constexpr res = Geom::VecWCS<2u>{4.0f, 6.0f};
        REQUIRE_THAT(vec_1 + vec_2, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec_1 = Geom::VecWCS<3u>{1.0f, 2.0f, 5.0f};
        auto constexpr vec_2 = Geom::VecWCS<3u>{3.0f, 4.0f, -7.0f};
        auto constexpr res = Geom::VecWCS<3u>{4.0f, 6.0f, -2.0f};
        REQUIRE_THAT(vec_1 + vec_2, AreEqualVecWCS(res));
    }

}

TEST_CASE("Returns the result of vector subtraction", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec_1 = Geom::VecWCS<2u>{1.0f, 7.0f};
        auto constexpr vec_2 = Geom::VecWCS<2u>{3.0f, 3.0f};
        auto constexpr res = Geom::VecWCS<2u>{-2.0f, 4.0f};
        REQUIRE_THAT(vec_1 - vec_2, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec_1 = Geom::VecWCS<3u>{7.0f, 2.0f, 5.0f};
        auto constexpr vec_2 = Geom::VecWCS<3u>{3.0f, 4.0f, -7.0f};
        auto constexpr res = Geom::VecWCS<3u>{4.0f, -2.0f, 12.0f};
        REQUIRE_THAT(vec_1 - vec_2, AreEqualVecWCS(res));
    }

}

TEST_CASE("Returns the result of element-wise vector multiplication", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec_1 = Geom::VecWCS<2u>{1.0f, 7.0f};
        auto constexpr vec_2 = Geom::VecWCS<2u>{3.0f, -3.0f};
        auto constexpr res = Geom::VecWCS<2u>{3.0f, -21.0f};
        REQUIRE_THAT(vec_1 * vec_2, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec_1 = Geom::VecWCS<3u>{7.0f, 2.0f, 5.0f};
        auto constexpr vec_2 = Geom::VecWCS<3u>{3.0f, 4.0f, -7.0f};
        auto constexpr res = Geom::VecWCS<3u>{21.0f, 8.0f, -35.0f};
        REQUIRE_THAT(vec_1 * vec_2, AreEqualVecWCS(res));
    }
}

TEST_CASE("Returns the result of element-wise vector division", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec_1 = Geom::VecWCS<2u>{1.0f, 14.0f};
        auto constexpr vec_2 = Geom::VecWCS<2u>{2.0f, -2.0f};
        auto constexpr res = Geom::VecWCS<2u>{0.5f, -7.0f};
        REQUIRE_THAT(vec_1 / vec_2, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec_1 = Geom::VecWCS<3u>{12.0f, 2.0f, 13.0f};
        auto constexpr vec_2 = Geom::VecWCS<3u>{6.0f, 4.0f, -1.0f};
        auto constexpr res = Geom::VecWCS<3u>{2.0f, 0.5f, -13.0f};
        REQUIRE_THAT(vec_1 / vec_2, AreEqualVecWCS(res));
    }
}

TEST_CASE("Returns the result of scalar addition", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec = Geom::VecWCS<2u>{1.0f, -14.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<2u>{3.0f, -12.0f};
        REQUIRE_THAT(vec + scalar, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec = Geom::VecWCS<3u>{1.0f, -14.0f, 4.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<3u>{3.0f, -12.0f, 6.0f};
        REQUIRE_THAT(vec + scalar, AreEqualVecWCS(res));
    }
}

TEST_CASE("Returns the result of scalar subtraction", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec = Geom::VecWCS<2u>{1.0f, -14.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<2u>{-1.0f, -16.0f};
        REQUIRE_THAT(vec - scalar, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec = Geom::VecWCS<3u>{1.0f, -14.0f, 4.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<3u>{-1.0f, -16.0f, 2.0f};
        REQUIRE_THAT(vec - scalar, AreEqualVecWCS(res));
    }
}

TEST_CASE("Returns the result of scalar multiplication", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec = Geom::VecWCS<2u>{1.0f, -14.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<2u>{2.0f, -28.0f};
        REQUIRE_THAT(vec * scalar, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec = Geom::VecWCS<3u>{1.0f, -14.0f, 4.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<3u>{2.0f, -28.0f, 8.0f};
        REQUIRE_THAT(vec * scalar, AreEqualVecWCS(res));
    }
}

TEST_CASE("Returns the result of scalar division", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec = Geom::VecWCS<2u>{1.0f, -14.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<2u>{0.5f, -7.0f};
        REQUIRE_THAT(vec / scalar, AreEqualVecWCS(res));
    }

    SECTION("3D") {
        auto constexpr vec = Geom::VecWCS<3u>{1.0f, -14.0f, 4.0f};
        auto constexpr scalar = 2.0f;
        auto constexpr res = Geom::VecWCS<3u>{0.5f, -7.0f, 2.0f};
        REQUIRE_THAT(vec / scalar, AreEqualVecWCS(res));
    }
}

TEST_CASE("Returns the norm of the vector", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec = Geom::VecWCS<2u>{3.0f, -4.0f};
        REQUIRE_THAT(norm(vec), AreEqualFloat(5.0f));
    }

    SECTION("3D") {
        auto constexpr vec = Geom::VecWCS<3u>{2.0f, 3.0f, -6.0f};
        REQUIRE_THAT(norm(vec), AreEqualFloat(7.0f));
    }
}

TEST_CASE("Returns the result of dot product", "[geom][vec_wcs_math]")
{
    using namespace GCT::Geom::CUDA;

    SECTION("2D") {
        auto constexpr vec_1 = Geom::VecWCS<2u>{3.0f, -4.0f};
        auto constexpr vec_2 = Geom::VecWCS<2u>{1.0f, -2.0f};
        REQUIRE_THAT(dot(vec_1, vec_2), AreEqualFloat(11.0f));
    }

    SECTION("3D") {
        auto constexpr vec_1 = Geom::VecWCS<3u>{3.0f, -4.0f, 3.0f};
        auto constexpr vec_2 = Geom::VecWCS<3u>{1.0f, -2.0f, 5.0f};
        REQUIRE_THAT(dot(vec_1, vec_2), AreEqualFloat(26.0f));
    }
}

}//GCT::Test namespace.
