#include <iostream>
#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/proj_geom.h"
#include "../../matchers.h"
#include "proj_geom_params_test_fix.h"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"
#include "proj_geom_test_fix.h"

namespace GCT::Test
{
TEST_CASE("Returns clockwise rotation direction",
          "[geom][proj_geom]")
{
    using namespace Geom::Units;
    SECTION("All angles in the first quadrant") {
        auto const rot_ang = std::vector{1.0_deg, 2.0_deg, 3.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CW);
    }
    SECTION("All angles in the 4th quadrant") {
        auto const rot_ang = std::vector{-10.0_deg, -9.0_deg, -8.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CW);
    }

    SECTION("First and second angles are in 4th and 1st quadrants") {
        auto const rot_ang = std::vector{358.0_deg, 0.0_deg, 2.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CW);
    }

    SECTION("First quadrant angles but the angles are not wrapped in the interval [0, 359)") {
        auto const rot_ang = std::vector{361.0_deg, 2.0_deg, 3.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CW);
    }
}

TEST_CASE("Returns counter-clockwise rotation direction",
          "[geom][proj_geom]")
{
    using namespace Geom::Units;
    SECTION("All angles in the first quadrant") {
        auto const rot_ang = std::vector{3.0_deg, 2.0_deg, 1.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CCW);
    }
    SECTION("All angles in the 4th quadrant") {
        auto const rot_ang = std::vector{-10.0_deg, -12.0_deg, -14.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CCW);
    }

    SECTION("First and second angles are in 1st and 4th quadrants") {
        auto const rot_ang = std::vector{2.0_deg, 0.0_deg, 358.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CCW);
    }

    SECTION("First quadrant angles but the angles are not wrapped in the interval [0, 359)") {
        auto const rot_ang = std::vector{3.0_deg, 361.0_deg, 359.0_deg};
        REQUIRE(Geom::Detail::rotDir(rot_ang) == Geom::RotDir::CCW);
    }
}

TEST_CASE("Does not return valid rotation direction",
          "[geom][proj_geom]")
{
    using namespace Geom::Units;
    SECTION("Empty rotation angle vector") {
        auto const rot_ang = std::vector<Radians>{};
        REQUIRE_FALSE(Geom::Detail::rotDir(rot_ang).has_value());
    }
    SECTION("First and second angles are same") {
        auto const rot_ang = std::vector{-10.0_deg, -10.0_deg, -14.0_deg};
        REQUIRE_FALSE(Geom::Detail::rotDir(rot_ang).has_value());
    }
}

TEST_CASE("Returns relative rotation angles",
          "[geom][proj_geom]")
{
    using namespace Geom::Units;

    SECTION("Clockwise rotation") {
        auto const rot_ang = std::vector{180.0_deg, 360.0_deg, 540.0_deg, 0.0_deg};
        auto const rel_ang = Geom::Detail::relRotAngles(rot_ang).value();
        auto const rel_ang_exp = std::vector{0.0_deg, 180.0_deg, 0.0_deg, 180.0_deg};
        REQUIRE_THAT(valArray(rel_ang), AreEqualFloatArrays(valArray(rel_ang_exp), 0.00001f));
    }

    SECTION("Counter-clockwise rotation") {
        auto const rot_ang = std::vector{0.0_deg, 540.0_deg, 360.0_deg, 180.0_deg};
        auto const rel_ang = Geom::Detail::relRotAngles(rot_ang).value();
        auto const rel_ang_exp = std::vector{0.0_deg, 180.0_deg, 0.0_deg, 180.0_deg};
        REQUIRE_THAT(valArray(rel_ang), AreEqualFloatArrays(valArray(rel_ang_exp), 0.00001f));
    }
    SECTION("180 degree") {
        auto const rot_ang = std::vector{90.0_deg, 270.0_deg, 90.0_deg};
        auto const rel_ang = Geom::Detail::relRotAngles(rot_ang).value();
        auto const rel_ang_exp = std::vector{0.0_deg, 180.0_deg, 0.0_deg};
        REQUIRE_THAT(valArray(rel_ang), AreEqualFloatArrays(valArray(rel_ang_exp), 0.00001f));
    }

    SECTION("Does not return valid values when all angles are zero") {
        auto const rot_ang = std::vector{0.0_deg, 360.0_deg, 0.0_deg, 360.0_deg};
        REQUIRE_FALSE(Geom::Detail::relRotAngles(rot_ang).has_value());
    }
}

TEST_CASE("Returns rotation angular increment in radians",
          "[geom][proj_geom]")
{
    using namespace Geom::Units;
    SECTION("Empty vector yields no value") {
        auto const rel_ang = std::vector<Radians>{};
        REQUIRE_FALSE(Geom::Detail::rotAngIncr(rel_ang).has_value());
    }
    SECTION("1 degree") {
        auto const rot_ang = std::vector{3.0_deg, 2.0_deg, 1.0_deg};
        auto const rel_rot_ang = Geom::Detail::relRotAngles(rot_ang).value();
        REQUIRE_THAT(Geom::Detail::rotAngIncr(rel_rot_ang).value(), AreEqualUnit(1.0_deg));

    }
    SECTION("180 degree") {
        auto const rot_ang = std::vector{90.0_deg, 270.0_deg, 90.0_deg};
        auto const rel_rot_ang = Geom::Detail::relRotAngles(rot_ang).value();
        REQUIRE_THAT(Geom::Detail::rotAngIncr(rel_rot_ang).value(), AreEqualUnit(180.0_deg));
    }
}

TEST_CASE("Returns rotation angular range", "[geom][proj_geom]")
{
    using namespace Geom::Units;

    SECTION("Empty vector yields no value") {
        auto const rel_ang = std::vector<Radians>{};
        REQUIRE_FALSE(Geom::Detail::rotAngRange(rel_ang).has_value());
    }
    SECTION("360 degree") {
        auto const rot_ang = std::vector{90.0_deg, 270.0_deg, 90.0_deg};
        auto const rel_ang = Geom::Detail::relRotAngles(rot_ang).value();
        REQUIRE_THAT(Geom::Detail::rotAngRange(rel_ang).value(), AreEqualUnit(360.0_deg));
    }
    SECTION("360 degree") {
        auto rot_ang = std::vector<Radians>(360);
        auto idx = 0.0f;
        std::for_each(std::begin(rot_ang), std::end(rot_ang),
                      [&idx](Radians &val)
                      {
                          val = val + 1.0_deg * idx;
                          idx++;
                      }
        );
        auto const rel_ang = Geom::Detail::relRotAngles(rot_ang).value();
        REQUIRE_THAT(Geom::Detail::rotAngRange(rel_ang).value(), AreEqualUnit(360.0_deg));
    }
}

TEST_CASE("Computes number of projections per rotation",
          "[geom][proj_geom]")
{
    using namespace Geom::Units;
    SECTION("No value exists") {
        auto const rel_rot_ang = std::vector<Radians>{};
        REQUIRE_FALSE(Geom::Detail::numProjPerRot(rel_rot_ang).has_value());
    }
    SECTION("No value exists since the first value is not zero") {
        auto const rel_rot_ang = std::vector<Radians>{Radians{1.0f}, Radians{2.0f}, Radians{3.0f}};
        REQUIRE_FALSE(Geom::Detail::numProjPerRot(rel_rot_ang).has_value());
    }
    SECTION("Returns 3") {
        auto const rel_rot_ang = std::vector{Radians{0.0f}, Radians{1.0f}, Radians{2.0f}};
        REQUIRE(Geom::Detail::numProjPerRot(rel_rot_ang).value() == 3u);
    }
    SECTION("Returns 2") {
        auto const rot_ang = std::vector{0.0_deg, 180.0_deg, 360.0_deg, 0.0_deg};
        auto const rel_rot_ang = Geom::Detail::relRotAngles(rot_ang).value();
        REQUIRE(Geom::Detail::numProjPerRot(rel_rot_ang).value() == 2u);
    }
}

TEMPLATE_TEST_CASE_SIG("Rotation info is extracted from the projection geometry",
                       "[geom][proj_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetParallel, 0u),
                       (Geom::DetFanArc, 0u),
                       (Geom::DetFanLine, 0u),
                       (Geom::DetParallel, 0u, Geom::DetParallel),
                       (Geom::DetParallel, 0u, Geom::DetFanLine),
                       (Geom::DetFanArc, 0u, Geom::DetFanLine),
                       (Geom::DetFanLine, 0u, Geom::DetFanLine))
{
    using namespace Geom;
    using namespace Units;
    using namespace Numeric::Comparator;
    auto const fix = ProjGeomTestFix<DetRow, DetCol...>{};
    auto const rot_info = Geom::getRotationGeom(fix.projGeom());
    auto const params = typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type{}.getParams();
    auto const eps = fix.cmpThreshold();

    REQUIRE(rot_info.has_value());
    auto const& vals = rot_info.value();
    REQUIRE(vals.dir == params.rot.dir);
    REQUIRE_THAT(vals.rot_ang_incr, AreEqualUnit(params.rot.ang_incr, eps));
    REQUIRE_THAT(vals.rot_ang_range, AreEqualUnit(360.0_deg, eps));
    REQUIRE(vals.num_proj_per_rot == params.rot.num_proj);
    REQUIRE_THAT(valArray(vals.rot_ang), AreEqualFloatArrays(fix.rotAng(), eps));
    REQUIRE_THAT(valArray(vals.rel_rot_ang), AreEqualFloatArrays(fix.relRotAng(), eps));
}

}//GCT::Test namespace.
