#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/projection_matrix.h"


TEST_CASE("Known projection geometry parameters are decomposed from the matrix",
          "[projection_matrix]")
{
    using namespace Eigen;
    Matrix<double, 3, 4> matrix;
    matrix << 2.48159483828274, 0.39171974522293, 0.0000, 307.5,
        0, 0.305095541401274, -2.48159483828274, 239.5,
        0, 0.00127388535031847, 0.0000, 1.0000;

    GCT::Geom::ProjectionMatrix pmat{matrix};
    constexpr auto pix_size = 0.616f;

    auto constexpr eps_non_zero = 0.000001f;
    auto constexpr eps_zero = 0.000001f;
    REQUIRE_THAT(pmat.principalPointDetRow(), Catch::Matchers::WithinRel(307.5f, eps_non_zero));
    REQUIRE_THAT(pmat.principalPointDetCol(), Catch::Matchers::WithinRel(239.5f, eps_non_zero));
    REQUIRE_THAT(pmat.focalLengthDetRow(), Catch::Matchers::WithinRel(1200.0f / pix_size, eps_non_zero));

    auto const src_pos = pmat.srcPos();
    auto const src_iso_dist_mm = src_pos.norm();
    REQUIRE_THAT(src_iso_dist_mm, Catch::Matchers::WithinRel(785.0f, eps_non_zero));
    REQUIRE_THAT(src_pos(0), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(src_pos(1), Catch::Matchers::WithinRel(-785.0f, eps_non_zero));
    REQUIRE_THAT(src_pos(2), Catch::Matchers::WithinAbs(0.0f, eps_zero));

    auto const rot_mat = pmat.rotationMatrix();
    REQUIRE_THAT(rot_mat(0, 0), Catch::Matchers::WithinAbs(1.0f, eps_non_zero));
    REQUIRE_THAT(rot_mat(0, 1), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(rot_mat(0, 2), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(rot_mat(1, 0), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(rot_mat(1, 1), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(rot_mat(1, 2), Catch::Matchers::WithinRel(-1.0f, eps_non_zero));
    REQUIRE_THAT(rot_mat(2, 0), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(rot_mat(2, 1), Catch::Matchers::WithinRel(1.0f, eps_non_zero));
    REQUIRE_THAT(rot_mat(2, 2), Catch::Matchers::WithinAbs(0.0f, eps_zero));

    auto const K = pmat.intrinsicMatrix();
    REQUIRE_THAT(K(0, 0), Catch::Matchers::WithinRel(1200.0f / pix_size, eps_non_zero));
    REQUIRE_THAT(K(0, 1), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(K(0, 2), Catch::Matchers::WithinRel(307.5f, eps_non_zero));
    REQUIRE_THAT(K(1, 0), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(K(1, 1), Catch::Matchers::WithinRel(1200.0f / pix_size, eps_non_zero));
    REQUIRE_THAT(K(2, 0), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(K(2, 1), Catch::Matchers::WithinAbs(0.0f, eps_zero));
    REQUIRE_THAT(K(2, 2), Catch::Matchers::WithinRel(1.0f, eps_non_zero));
}


