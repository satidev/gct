#include "catch.hpp"
#include "../../../src/cpp/geom/detector_1d.h"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"
#include "../../../src/cpp/numeric/comparator.h"
#include "proj_geom_params_test_fix.h"
#include "../../../src/cpp/geom/geom_math.h"
#include "../../matchers.h"

namespace GCT::Test
{

TEST_CASE_METHOD(Parallel1DTestParams,
                 "1D parallel projection geometry is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetParallel;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow>::create(params, 2u);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(pg.rot_ang == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow>::create(params, 3u);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(pg.rot_ang == Units::Radians{2.0f * std::numbers::pi_v<float>});
    }
}

TEST_CASE_METHOD(FanBeamTestParams,
                 "Fan-beam projection geometry for arc-shaped detector is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetFanArc;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow>::create(params, 2u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Geom::Math::norm(pg.src_pos));
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(Detail::rotAng(pg.src_pos) == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});

        //The source is at y-axis.
        auto const src_neg_y = VecWCS<2u>{0.0f, -params.src_iso_dist.val};
        REQUIRE(areEqual(pg.src_pos, src_neg_y, this->eps));

        //Detector row direction is parallel to the x-axis of WCS.
        auto const x_axis = VecWCS<2u>{1.0f, 0.0f};
        REQUIRE(areEqual(pg.det_row_dir, x_axis, eps));

        //Source-detector direction is parallel to the y-axis of WCS.
        auto const y_axis = VecWCS<2u>{0.0f, 1.0f};
        REQUIRE(areEqual(pg.iso_ray_dir, y_axis, eps));
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow>::create(params, 3u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Approx{Geom::Math::norm(pg.src_pos)});
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE_THAT(Detail::rotAng(pg.src_pos).val, AreEqualFloat(0.0f, eps));
        REQUIRE_THAT(Detail::rotAng(pg.src_pos).val, AreEqualFloat(0.0f, eps));

        //Source situated on x-axis.
        auto const src_x = VecWCS<2u>{params.src_iso_dist.val, 0.0f};
        REQUIRE(areEqual(pg.src_pos, src_x, eps));

        //Detector row direction is parallel to y-axis of WCS.
        auto const y_axis = VecWCS<2u>{0.0f, 1.0f};
        REQUIRE(areEqual(pg.det_row_dir, y_axis, eps));

        //Source-detector direction is anti-parallel to the x-axis of WCS.
        auto const neg_x_axis = VecWCS<2u>{-1.0f, 0.0f};
        REQUIRE(areEqual(pg.iso_ray_dir, neg_x_axis, eps));
    }
}

TEST_CASE_METHOD(FanBeamTestParams,
                 "Fan-beam projection geometry for line-shaped detector is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetFanLine;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow>::create(params, 2u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Approx{Geom::Math::norm(pg.src_pos)});
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(Detail::rotAng(pg.src_pos) == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});

        //The source is at y-axis.
        auto const src_neg_y = VecWCS<2u>{0.0f, -params.src_iso_dist.val};
        REQUIRE(areEqual(pg.src_pos, src_neg_y, this->eps));

        //Detector row direction is parallel to the x-axis of WCS.
        auto const x_axis = VecWCS<2u>{1.0f, 0.0f};
        REQUIRE(areEqual(pg.det_row_dir, x_axis, eps));

        //Source-detector direction is parallel to the y-axis of WCS.
        auto const y_axis = VecWCS<2u>{0.0f, 1.0f};
        REQUIRE(areEqual(Geom::Math::rot90(pg.det_row_dir), y_axis, eps));
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow>::create(params, 3u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Approx{Geom::Math::norm(pg.src_pos)});
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE_THAT(Detail::rotAng(pg.src_pos).val, AreEqualFloat(0.0f, eps));

        //Source situated on x-axis.
        auto const src_x = VecWCS<2u>{params.src_iso_dist.val, 0.0f};
        REQUIRE(areEqual(pg.src_pos, src_x, eps));

        //Detector row direction is parallel to y-axis of WCS.
        auto const y_axis = VecWCS<2u>{0.0f, 1.0f};
        REQUIRE(areEqual(pg.det_row_dir, y_axis, eps));

        //Source-detector direction is anti-parallel to the x-axis of WCS.
        auto const neg_x_axis = VecWCS<2u>{-1.0f, 0.0f};
        REQUIRE(areEqual(Geom::Math::rot90(pg.det_row_dir), neg_x_axis, eps));
    }
}

TEST_CASE_METHOD(Parallel2DTestParams,
                 "2D parallel projection geometry is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetParallel;
    using DetCol = DetParallel;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 2u);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(pg.rot_ang == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 3u);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(pg.rot_ang == Units::Radians{2.0f * std::numbers::pi_v<float>});
    }
}

TEST_CASE_METHOD(ConeBeamTestParams,
                 "Cone-parallel projection geometry for rebinned detector is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetParallel;
    using DetCol = DetFanLine;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 2u);
        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist == pg.src_iso_dist);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(pg.rot_ang == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 3u);
        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist == pg.src_iso_dist);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(pg.rot_ang == Units::Radians{2.0f * std::numbers::pi_v<float>});
    }
}

TEST_CASE_METHOD(ConeBeamTestParams,
                 "Cone-beam projection geometry for cylindrical detector is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetFanArc;
    using DetCol = DetFanLine;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 2u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Approx{Geom::Math::norm(pg.src_pos)});
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(Detail::rotAng(pg.src_pos) == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});

        //Source is at -y-axis.
        auto const src_neg_y = VecWCS<3u>{0.0f, -params.src_iso_dist.val, 0.0f};
        REQUIRE(areEqual(pg.src_pos, src_neg_y, this->eps));

        //Detector row direction is parallel to the x-axis of WCS.
        auto const x_axis = VecWCS<3u>{1.0f, 0.0f, 0.0f};
        REQUIRE(areEqual(pg.det_row_dir, x_axis, eps));

        //Detector column direction is anti-parallel to the z-axis of WCS.
        auto const neg_z_axis = VecWCS<3u>{0.0f, 0.0f, -1.0f};
        REQUIRE(areEqual(pg.det_col_dir, neg_z_axis, eps));

        //Source-detector direction is parallel to the y-axis of WCS.
        auto const y_axis = VecWCS<3u>{0.0f, 1.0f, 0.0f};
        REQUIRE(areEqual(pg.iso_ray_dir, y_axis, eps));
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 3u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Geom::Math::norm(pg.src_pos));
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE_THAT(Detail::rotAng(pg.src_pos).val, AreEqualFloat(0.0f, eps));

        //Source situated on x-axis.
        auto const src_x = VecWCS<3u>{params.src_iso_dist.val, 0.0f, 0.0f};
        REQUIRE(areEqual(pg.src_pos, src_x, eps));

        //Detector row direction is parallel to y-axis of WCS.
        auto const y_axis = VecWCS<3u>{0.0f, 1.0f, 0.0f};
        REQUIRE(areEqual(pg.det_row_dir, y_axis, eps));

        //Detector column direction is anti-parallel to the z-axis of WCS.
        auto const neg_z_axis = VecWCS<3u>{0.0f, 0.0f, -1.0f};
        REQUIRE(areEqual(pg.det_col_dir, neg_z_axis, eps));

        //Source-detector direction is anti-parallel to the x-axis of WCS.
        auto const neg_x_axis = VecWCS<3u>{-1.0f, 0.0f, 0.0f};
        REQUIRE(areEqual(pg.iso_ray_dir, neg_x_axis, eps));
    }
}

TEST_CASE_METHOD(ConeBeamTestParams,
                 "Cone-beam projection geometry for flat detector is created from parameters",
                 "[proj_geom_factory_params]")
{
    using namespace Geom;
    using namespace Numeric::Comparator;
    using DetRow = DetFanLine;
    using DetCol = DetFanLine;

    SECTION("Third projection: rotation angle 270 degree") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 2u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Approx{Geom::Math::norm(pg.src_pos)});
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE(Detail::rotAng(pg.src_pos) == Units::Radians{3.0f * std::numbers::pi_v<float> / 2.0f});

        //The Source is at y-axis.
        auto const src_neg_y = VecWCS<3u>{0.0f, -params.src_iso_dist.val, 0.0f};
        REQUIRE(areEqual(pg.src_pos, src_neg_y, this->eps));

        //Detector row direction is parallel to the x-axis of WCS.
        auto const x_axis = VecWCS<3u>{1.0f, 0.0f, 0.0f};
        REQUIRE(areEqual(pg.det_row_dir, x_axis, eps));

        //Detector column direction is antiparallel to the z-axis of WCS.
        auto const neg_z_axis = VecWCS<3u>{0.0f, 0.0f, -1.0f};
        REQUIRE(areEqual(pg.det_col_dir, neg_z_axis, eps));

        //Source-detector direction is parallel to the y-axis of WCS.
        auto const y_axis = VecWCS<3u>{0.0f, 1.0f, 0.0f};
        REQUIRE(areEqual(Geom::Math::crossProduct(pg.det_row_dir, pg.det_col_dir),
                         y_axis, eps));
    }

    SECTION("Fourth projection: rotation angle 0") {
        auto const pg = ProjGeomFactory<DetRow, DetCol>::create(params, 3u);

        REQUIRE(params.src_det_dist == pg.src_det_dist);
        REQUIRE(params.src_iso_dist.val == Approx{Geom::Math::norm(pg.src_pos)});
        REQUIRE(params.det_col_center == pg.det_col_center);
        REQUIRE(params.det_row_center == pg.det_row_center);
        REQUIRE_THAT(Detail::rotAng(pg.src_pos).val, AreEqualFloat(0.0f, eps));

        //Source situated on x-axis.
        auto const src_x = VecWCS<3u>{params.src_iso_dist, 0.0f, 0.0f};
        REQUIRE(areEqual(pg.src_pos, src_x, eps));

        //Detector row direction is parallel to the y-axis of WCS.
        auto const y_axis = VecWCS<3u>{0.0f, 1.0f, 0.0f};
        REQUIRE(areEqual(pg.det_row_dir, y_axis, eps));

        //Detector column direction is anti-parallel to the z-axis of WCS.
        auto const neg_z_axis = VecWCS<3u>{0.0f, 0.0f, -1.0f};
        REQUIRE(areEqual(pg.det_col_dir, neg_z_axis, eps));

        //Source-detector direction is anti-parallel to x-axis of WCS.
        auto const neg_x_axis = VecWCS<3u>{-1.0f, 0.0f, 0.0f};
        REQUIRE(areEqual(Geom::Math::crossProduct(pg.det_row_dir, pg.det_col_dir),
                         neg_x_axis, eps));
    }
}

}//GCT::Test namespace
