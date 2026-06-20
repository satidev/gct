#include "../../../thirdparty/catch.hpp"
#include "../../matchers.h"
#include "../../../src/cpp/geom/units.h"

namespace GCT::Test
{
TEST_CASE("User defined literal for radians", "[geom][units]")
{
    using namespace GCT::Geom::Units;

    SECTION("0 radian") {
        auto const val = 0.0_rad;
        REQUIRE(val == Radians{0.0f});
    }

    SECTION("1 radian") {
        auto const val = 1.0_rad;
        REQUIRE(val == Radians{1.0f});
    }
}

TEST_CASE("User defined literal for degrees", "[geom][units]")
{
    using namespace GCT::Geom::Units;

    SECTION("0 degree is mapped to 0 radians") {
        auto const val = 0.0_deg;
        REQUIRE(val == Radians{0.0f});
    }

    SECTION("180 degree is mapped to PI radians") {
        auto const val = 180.0_deg;
        REQUIRE(val == Radians{std::numbers::pi_v<float>});
    }

    SECTION("360 degree is mapped to 2*PI radians") {
        auto const val = 360.0_deg;
        REQUIRE(val == Radians{2.0f * std::numbers::pi_v<float>});
    }
}

TEST_CASE("User defined literal for millimeters", "[geom][units]")
{
    using namespace GCT::Geom::Units;

    SECTION("0 mm") {
        auto const val = 0.0_mm;
        REQUIRE(val == MilliMeters{0.0f});
    }

    SECTION("1000 mm") {
        auto const val = 1000.0_mm;
        REQUIRE(val == MilliMeters{1000.0f});
    }
}

TEST_CASE("User defined literal for detector pixels", "[geom][units]")
{
    using namespace GCT::Geom::Units;

    SECTION("0 pixels") {
        auto const val = 0.0_pix;
        REQUIRE(val == DetPixels{0.0f});
    }

    SECTION("1000 pixels") {
        auto const val = 1000.0_pix;
        REQUIRE(val == DetPixels{1000.0f});
    }
}

TEMPLATE_TEST_CASE("Returns true since the two units are equal",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE(TestType{1.0f} == TestType{1.0f});
}

TEMPLATE_TEST_CASE("Returns false since the two units are not equal",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE_FALSE(TestType{1.0f} == TestType{-1.0f});
}

TEMPLATE_TEST_CASE("Returns false since the two units are equal",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE_FALSE(TestType{1.0f} != TestType{1.0f});
}

TEMPLATE_TEST_CASE("Returns true since the two units are not equal",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE(TestType{1.0f} != TestType{-1.0f});
}

TEMPLATE_TEST_CASE("Returns true since the first value is less than second",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE(TestType{1.0f} < TestType{2.0f});
}

TEMPLATE_TEST_CASE("Returns true since the first value is greater than second",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE(TestType{2.0f} > TestType{1.0f});
}

TEMPLATE_TEST_CASE("Returns negative value of the first",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE(-TestType{2.0f} == TestType{-2.0f});
}

TEMPLATE_TEST_CASE("Returns 5 as sum",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE((TestType{2.0f} + TestType{3.0f}) == TestType{5.0f});
    REQUIRE((TestType{2.0f} + 3.0f) == TestType{5.0f});
}

TEMPLATE_TEST_CASE("Returns 3 as difference",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE((TestType{5.0f} - TestType{2.0f}) == TestType{3.0f});
    REQUIRE((TestType{5.0f} - 2.0f) == TestType{3.0f});
}

TEMPLATE_TEST_CASE("Returns number 6.0 after multiplication",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE((TestType{3.0f} * 2.0f) == TestType{6.0f});
}

TEMPLATE_TEST_CASE("Returns number 3.0 after division",
                   "[geom][units]",
                   Geom::Units::MilliMeters,
                   Geom::Units::DetPixels,
                   Geom::Units::Radians)
{
    REQUIRE((TestType{6.0f} / TestType{2.0f}) == 3.0f);
}

}//GCT::Test namespace.
