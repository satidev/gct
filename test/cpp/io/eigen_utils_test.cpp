#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/geom/eigen_utils.h"

TEST_CASE("Eigen 3x1 vector to Vec3DWCS", "[eigen_utils]")
{
    auto const vec_eigen = Eigen::Vector3f{0.0f, 1.0f, -2.0f};
    auto const vec_wcs = GCT::Geom::EigenUtils::getVecWCS3D(vec_eigen);

    REQUIRE(vec_wcs.x.val == Approx{0.0f});
    REQUIRE(vec_wcs.y.val == Approx{1.0f});
    REQUIRE(vec_wcs.z.val == Approx{-2.0f});
}

TEST_CASE("std::array to Eigen matrix", "[eigen_utils]")
{
    auto constexpr num_rows = 3;
    auto constexpr num_cols = 4;
    auto const arr = std::array<double, num_rows * num_cols>{
        2.48159483828274, 0.39171974522293, 0.0000, 307.5,
        0, 0.305095541401274, -2.48159483828274, 239.5,
        0, 0.00127388535031847, 0.0000, 1.0000
    };

    auto const mat = GCT::Geom::EigenUtils::arrToMat<double, num_rows, num_cols>(arr);
    for (int row_idx = 0; row_idx < mat.rows(); row_idx++)
        for (int col_idx = 0; col_idx < mat.cols(); col_idx++){
            auto const idx = static_cast<unsigned>(row_idx * num_cols + col_idx);
            REQUIRE(mat.coeff(row_idx, col_idx) == Approx{arr[idx]});
        }
}
