#include <iostream>
#include "catch.hpp"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"
#include "../../../src/cpp/geom/projection_matrix.h"
#include "../../../src/cpp/numeric/comparator.h"
#include "../../../src/cpp/geom/rotation_geom.h"

namespace GCT::Test
{
TEST_CASE("Known projection geometry parameters are computed from the matrix (eigen)", "[proj_geom_pmat]")
{
    using namespace Eigen;
    using namespace GCT::Geom;

    Matrix<double, 3, 4> matrix;
    matrix << 2.48159483828274, 0.39171974522293, 0.0000, 307.5,
        0, 0.305095541401274, -2.48159483828274, 239.5,
        0, 0.00127388535031847, 0.0000, 1.0000;
    constexpr auto pix_size = 0.616f;

    //Decomposition
    auto const pmat_dec = ProjectionMatrix{matrix};
    auto pg = ProjGeomFactory<DetFanLine, DetFanLine>::create(pmat_dec);
    pg.src_det_dist.val *= pix_size;

    REQUIRE(Detail::rotAng(pg.src_pos) == Units::Radians{std::numbers::pi_v<float> * 3.0f / 2.0f});
    using namespace Geom::Units;
    REQUIRE(pg.src_det_dist == 1200.0_mm);
    REQUIRE(pg.det_row_center == Units::DetPixels{307.5f});
    REQUIRE(pg.det_col_center == Units::DetPixels{239.5f});

    using namespace GCT::Numeric::Comparator;
    constexpr auto eps = 0.00000001f;
    //REQUIRE(areEqual(pg.src_pos, GCT::Geom::VecWCS<3u>{0.0f, -pg.src_iso_dist_mm, 0.0f}, eps));
    REQUIRE(areEqual(pg.det_row_dir, GCT::Geom::VecWCS<3u>{1.0f, 0.0f, 0.0f}, eps));
    REQUIRE(areEqual(pg.det_col_dir, GCT::Geom::VecWCS<3u>{0.0f, 0.0f, -1.0f}, eps));
    //REQUIRE(areEqual(, GCT::Geom::VecWCS<3u>{0.0f, 1.0f, 0.0f}, eps));


    //Compose projection matrix from projection geometry parameters.
    auto const pmat_comp = ProjectionMatrix{pg, pix_size, pix_size};

    REQUIRE(pmat_dec.focalLengthDetRow() == Approx{pmat_comp.focalLengthDetRow()});
    REQUIRE(pmat_dec.principalPointDetRow() == Approx{pmat_comp.principalPointDetRow()});
    REQUIRE(pmat_dec.principalPointDetCol() == Approx{pmat_comp.principalPointDetCol()});
    //REQUIRE(pmat_dec.srcPos() == pmat_comp.srcPos());
    //REQUIRE(pmat_dec.rotationMatrix() == pmat_comp.rotationMatrix());
    //REQUIRE(pmat_dec.intrinsicMatrix() == pmat_comp.intrinsicMatrix());
}



}//GCT::Test namespace.



