#include "../../../src/cpp/numeric/comparator.h"
#include "../../../thirdparty/catch.hpp"
#include <array>

namespace GCT::Test
{
using namespace Numeric::Comparator;

TEST_CASE("Returns true since the input is positive infinity",
          "[numeric][comparator]")
{
    REQUIRE(isPositiveInf(std::numeric_limits<float>::infinity()));
}
TEST_CASE("Returns false since the input is not positive infinity",
          "[numeric][comparator]")
{
    REQUIRE_FALSE(isPositiveInf(std::numeric_limits<float>::epsilon()));
    REQUIRE_FALSE(isPositiveInf(-1.0f * std::numeric_limits<float>::infinity()));
    REQUIRE_FALSE(isPositiveInf(std::numeric_limits<float>::quiet_NaN()));
}

TEST_CASE("Returns true since the input is negative infinity",
          "[numeric][comparator]")
{
    REQUIRE(isNegativeInf(-1.0f * std::numeric_limits<float>::infinity()));
}

TEST_CASE("Returns false since the input is not negative infinity",
          "[numeric][comparator]")
{
    REQUIRE_FALSE(isNegativeInf(std::numeric_limits<float>::epsilon()));
    REQUIRE_FALSE(isNegativeInf(std::numeric_limits<float>::infinity()));
    REQUIRE_FALSE(isNegativeInf(std::numeric_limits<float>::quiet_NaN()));
}

TEST_CASE("Returns true since the two numbers are equal",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        REQUIRE(areEqual(2.0f, 2.0f, eps));
        REQUIRE(areEqual(0.0f, 0.0f, eps));
        REQUIRE(areEqual(-0.0f, 0.0f, eps));

        REQUIRE(areEqual(std::numeric_limits<float>::infinity(),
                         std::numeric_limits<float>::infinity(),
                         eps));
        REQUIRE(areEqual(-1.0f*std::numeric_limits<float>::infinity(),
                         -1.0f*std::numeric_limits<float>::infinity(),
                         eps));
        REQUIRE(areEqual(std::numeric_limits<float>::quiet_NaN(),
                         std::numeric_limits<float>::quiet_NaN(),
                         eps));

        REQUIRE(areEqualAbs(2.0f, 2.0f, eps));
        REQUIRE(areEqualAbs(0.0f, 0.0f, eps));
        REQUIRE(areEqualAbs(-0.0f, 0.0f, eps));

        REQUIRE(areEqualRel(2.0f, 2.0f, eps));
        REQUIRE(areEqualRel(0.0f, 0.0f, eps));
        REQUIRE(areEqualRel(-0.0f, 0.0f, eps));
    }
    SECTION("Integral values") {
        REQUIRE(areEqual(1, 1));
        REQUIRE(areEqual(false, false));
        REQUIRE(areEqual(345u, 345u));
    }
}

TEST_CASE("Returns true by the relative comparison but not by absolute",
          "[numeric][comparator]")
{
    auto constexpr eps = 0.00000000001;
    REQUIRE_FALSE(areEqualAbs(2333333.555555555, 2333333.555555556, eps));
    REQUIRE(areEqualRel(2333333.555555555, 2333333.555555556, eps));
    REQUIRE(areEqual(2333333.555555555, 2333333.555555556, eps));
}

TEST_CASE("Returns true by the absolute comparison but not by relative",
          "[numeric][comparator]")
{
    auto constexpr eps = 0.00000001f;
    REQUIRE(areEqualAbs(0.0f, 1.55672e-13f, eps));
    REQUIRE_FALSE(areEqualRel(0.0f, 1.55672e-13f, eps));
    REQUIRE(areEqual(0.0f, 1.55672e-13f, eps));
}

TEST_CASE("Returns false since the two numbers are not equal",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        REQUIRE_FALSE(areEqual(2.0f, -2.0f, eps));
    }
    SECTION("Integral values") {
        REQUIRE_FALSE(areEqual(145u, 1567u));
        REQUIRE_FALSE(areEqual(false, true));
        REQUIRE_FALSE(areEqual(-10, 10));
    }
}

TEST_CASE("Returns true since the two VecWCS objects are equal",
          "[numeric][comparator]")
{
    auto constexpr eps = 0.00001f;
    SECTION("3D") {
        REQUIRE(areEqual(GCT::Geom::VecWCS<3u>{1.0f, 2.0f, -3.0f},
                         GCT::Geom::VecWCS<3u>{1.0f, 2.0f, -3.0f},
                         eps));
    }
    SECTION("2D") {
        REQUIRE(areEqual(GCT::Geom::VecWCS<2u>{1.0f, 2.0f},
                         GCT::Geom::VecWCS<2u>{1.0f, 2.0f},
                         eps));
    }
}

TEST_CASE("Returns false since the two VecWCS objects are not equal",
          "[numeric][comparator]")
{
    auto constexpr eps = 0.00001f;
    SECTION("3D") {
        REQUIRE_FALSE(areEqual(GCT::Geom::VecWCS<3u>{1.0f, 2.0f, -3.0f},
                               GCT::Geom::VecWCS<3u>{1.0f, 2.0f, 3.0f},
                               eps));
    }
    SECTION("2D") {
        REQUIRE_FALSE(areEqual(GCT::Geom::VecWCS<2u>{1.0f, 2.0f},
                               GCT::Geom::VecWCS<2u>{1.0f, -2.0f},
                               eps));
    }
}

TEST_CASE("Returns true since the container has the same elements",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        auto const vec = std::vector{1.0f, 1.0f, 1.0f};
        REQUIRE(areAllElemsSame<float>(vec, 1.0f, eps));
    }
    SECTION("Integral values") {
        auto constexpr arr = std::array{1u, 1u, 1u};
        REQUIRE(areAllElemsSame<unsigned>(arr, 1u));
    }
}

TEST_CASE("Returns false since all the elements of the container are not equal",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        auto const vec = std::vector{1.0f, 1.0f, 1.0f};
        REQUIRE_FALSE(areAllElemsSame<float>(vec, 2.0f, eps));

    }
    SECTION("Integral values") {
        auto constexpr arr = std::array{1u, 1u, 1u};
        REQUIRE_FALSE(areAllElemsSame<unsigned >(arr, 2u));
    }
}

TEST_CASE("Returns true since the two containers are equal",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        auto const vec = std::vector{1.0f, 1.0f, 1.0f};
        REQUIRE(areEqualArrays<float>(vec, std::vector{1.0f, 1.0f, 1.0f}, eps));
    }
    SECTION("Integral values") {
        auto constexpr arr = std::array<uint, 3>{1u, 1u, 1u};
        REQUIRE(areEqualArrays<unsigned>(arr, std::array{1u, 1u, 1u}));
    }
}

TEST_CASE("Returns false since the two containers are not equal",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        auto const vec = std::vector{1.0f, 1.0f, 1.0f};
        REQUIRE_FALSE(areEqualArrays<float>(vec, std::vector{2.0f, 1.0f, 1.0f}, eps));
    }
    SECTION("Integral values") {
        auto constexpr arr = std::array{1u, 1u, 1u};
        REQUIRE_FALSE(areEqualArrays<unsigned>(arr, std::array{2u, 1u, 1u}));
    }
}

TEST_CASE("Returns false since the containers are of different sizes",
          "[numeric][comparator]")
{
    SECTION("Floating point values") {
        auto constexpr eps = 0.00001f;
        auto const vec = std::vector{1.0f, 1.0f, 1.0f};
        REQUIRE_FALSE(areEqualArrays<float>(std::vector{1.0f, 1.0f}, vec, eps));
    }
    SECTION("Integral values") {
        auto constexpr arr = std::array{1u, 1u, 1u};
        REQUIRE_FALSE(areEqualArrays<unsigned>(std::array<unsigned, 3>{1u, 1u}, arr));
    }
}
}//GCT::Test namespace.






