#include "../../../thirdparty/catch.hpp"
#include "../../matchers.h"
#include "../../../src/cpp/geom/utils.h"

namespace GCT::Test
{
struct GeomUtilsTestFix
{
    static constexpr auto pi_f = std::numbers::pi_v<float>;
    static constexpr auto eps_f = 0.0000000001f;
    static constexpr auto pi_d = std::numbers::pi;
    static constexpr auto eps_d = 0.000000000000001;
};

TEST_CASE_METHOD(GeomUtilsTestFix, "Degree to radian conversion", "[geom][utils]")
{
    SECTION("0 is mapped to 0") {
        REQUIRE_THAT(Geom::Utils::degToRad(0.0f), AreEqualFloat(0.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::degToRad(0.0), AreEqualFloat(0.0, eps_d));
    }
    SECTION("90 is mapped to PI/2") {
        REQUIRE_THAT(Geom::Utils::degToRad(90.0f), AreEqualFloat(pi_f/2.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::degToRad(90.0), AreEqualFloat(pi_d/2.0, eps_d));
    }
    SECTION("180 is mapped to PI") {
        REQUIRE_THAT(Geom::Utils::degToRad(180.0f), AreEqualFloat(pi_f, eps_f));
        REQUIRE_THAT(Geom::Utils::degToRad(180.0), AreEqualFloat(pi_d, eps_d));
    }
    SECTION("270 is mapped to 3*PI/2") {
        REQUIRE_THAT(Geom::Utils::degToRad(270.0f), AreEqualFloat(3*pi_f/2, eps_f));
        REQUIRE_THAT(Geom::Utils::degToRad(270.0), AreEqualFloat(3*pi_d/2, eps_d));
    }
    SECTION("360 is mapped to 2*PI") {
        REQUIRE_THAT(Geom::Utils::degToRad(360.0f), AreEqualFloat(2*pi_f, eps_f));
        REQUIRE_THAT(Geom::Utils::degToRad(360.0), AreEqualFloat(2*pi_d, eps_d));
    }
}

TEST_CASE_METHOD(GeomUtilsTestFix, "Radian to degree conversion", "[geom][utils]")
{
    SECTION("0 is mapped to 0") {
        REQUIRE_THAT(Geom::Utils::radToDeg(0.0f), AreEqualFloat(0.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::radToDeg(0.0), AreEqualFloat(0.0, eps_d));
    }
    SECTION("PI/2 is mapped to 90") {
        REQUIRE_THAT(Geom::Utils::radToDeg(pi_f/2.0f), AreEqualFloat(90.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::radToDeg(pi_d/2.0), AreEqualFloat(90.0, eps_d));
    }
    SECTION("PI is mapped to 180") {
        REQUIRE_THAT(Geom::Utils::radToDeg(pi_f), AreEqualFloat(180.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::radToDeg(pi_d), AreEqualFloat(180.0, eps_d));
    }
    SECTION("3*PI/2 is mapped to 270") {
        REQUIRE_THAT(Geom::Utils::radToDeg(3*pi_f/2), AreEqualFloat(270.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::radToDeg(3*pi_d/2), AreEqualFloat(270.0, eps_d));
    }
    SECTION("2*PI is mapped to 360") {
        REQUIRE_THAT(Geom::Utils::radToDeg(2*pi_f), AreEqualFloat(360.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::radToDeg(2*pi_d), AreEqualFloat(360.0, eps_d));
    }
}

TEST_CASE_METHOD(GeomUtilsTestFix, "Wrap angle to the interval [0, 2*PI)", "[geom][utils]")
{
    SECTION("0 is mapped to 0") {
        REQUIRE_THAT(Geom::Utils::wrapAngle(0.0f), AreEqualFloat(0.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::wrapAngle(0.0), AreEqualFloat(0.0, eps_d));
        REQUIRE_THAT(Geom::Utils::wrapFrom0To2PI(Geom::Units::Radians{0.0f}).val,
                     AreEqualFloat(0.0f, eps_f));
    }

    SECTION("PI/2 is mapped to PI/2") {
        REQUIRE_THAT(Geom::Utils::wrapAngle(pi_f/2), AreEqualFloat(pi_f/2, eps_f));
        REQUIRE_THAT(Geom::Utils::wrapAngle(pi_d/2), AreEqualFloat(pi_d/2, eps_d));
        REQUIRE_THAT(Geom::Utils::wrapFrom0To2PI(Geom::Units::Radians{pi_f/2}).val,
                     AreEqualFloat(pi_f/2, eps_f));
    }
    SECTION("-PI/2 is mapped to 3*PI/2") {
        REQUIRE_THAT(Geom::Utils::wrapAngle(-pi_f/2), AreEqualFloat(3*pi_f/2, eps_f));
        REQUIRE_THAT(Geom::Utils::wrapAngle(-pi_d/2), AreEqualFloat(3*pi_d/2, eps_d));
        REQUIRE_THAT(Geom::Utils::wrapFrom0To2PI(Geom::Units::Radians{-pi_f/2}).val,
                     AreEqualFloat(3*pi_f/2, eps_f));
    }
    SECTION("-PI is mapped to PI") {
        REQUIRE_THAT(Geom::Utils::wrapAngle(-pi_f), AreEqualFloat(pi_f, eps_f));
        REQUIRE_THAT(Geom::Utils::wrapAngle(-pi_d), AreEqualFloat(pi_d, eps_d));
        REQUIRE_THAT(Geom::Utils::wrapFrom0To2PI(Geom::Units::Radians{-pi_f}).val,
                     AreEqualFloat(pi_f, eps_f));
    }
    SECTION("2*PI is mapped to 0") {
        REQUIRE_THAT(Geom::Utils::wrapAngle(2*pi_f), AreEqualFloat(0.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::wrapAngle(2*pi_d), AreEqualFloat(0.0, eps_d));
        REQUIRE_THAT(Geom::Utils::wrapFrom0To2PI(Geom::Units::Radians{2*pi_f}).val,
                     AreEqualFloat(0.0f, eps_f));
    }
    SECTION("-2*PI is mapped to 0") {
        REQUIRE_THAT(Geom::Utils::wrapAngle(-2*pi_f), AreEqualFloat(0.0f, eps_f));
        REQUIRE_THAT(Geom::Utils::wrapAngle(-2*pi_d), AreEqualFloat(0.0, eps_d));
        REQUIRE_THAT(Geom::Utils::wrapFrom0To2PI(Geom::Units::Radians{-2*pi_f}).val,
                     AreEqualFloat(0.0f, eps_f));
    }
}

TEST_CASE("Computes the quadrant based on rotation angle", "[geom][utils]")
{
    using namespace Geom::Utils;
    using namespace Geom::Units;
    SECTION("0 and 45 degrees lie in the first quadrant") {
        REQUIRE(quadrantIndex(degToRad(0.0f)) == 1u);
        REQUIRE(quadrantIndex(degToRad(45.0f)) == 1u);

        REQUIRE(quadrantIndex(0.0_deg) == 1u);
        REQUIRE(quadrantIndex(45.0_deg) == 1u);
    }
    SECTION("90 and 135 degrees lie in the second quadrant") {
        REQUIRE(quadrantIndex(degToRad(90.0f)) == 2u);
        REQUIRE(quadrantIndex(degToRad(135.0f)) == 2u);

        REQUIRE(quadrantIndex(90.0_deg) == 2u);
        REQUIRE(quadrantIndex(135.0_deg) == 2u);
    }
    SECTION("180 and 190 degrees lie in the third quadrant") {
        REQUIRE(quadrantIndex(degToRad(180.0f)) == 3u);
        REQUIRE(quadrantIndex(degToRad(190.0f)) == 3u);

        REQUIRE(quadrantIndex(180.0_deg) == 3u);
        REQUIRE(quadrantIndex(190.0_deg) == 3u);
    }
    SECTION("270 and 334 degrees lie in the fourth quadrant") {
        REQUIRE(quadrantIndex(degToRad(270.0f)) == 4u);
        REQUIRE(quadrantIndex(degToRad(334.0f)) == 4u);

        REQUIRE(quadrantIndex(270.0_deg) == 4u);
        REQUIRE(quadrantIndex(334.0_deg) == 4u);
    }
    SECTION("-45 degree lies in the fourth quadrant") {
        REQUIRE(quadrantIndex(degToRad(-45.0f)) == 4u);
        REQUIRE(quadrantIndex(-45.0_deg) == 4u);
    }
}

}//GCT::Test namespace.