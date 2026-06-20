#ifndef EIGEN_UTILS_H
#define EIGEN_UTILS_H

#include "vec_wcs.h"
#include <Eigen/Dense>

namespace GCT::Geom::EigenUtils
{
Geom::VecWCS<3u> getVecWCS3D(Eigen::Vector3f const &vec);

template<int num_rows, int num_cols>
Eigen::Matrix<double, num_rows, num_cols> arrToMat(std::array<double, num_rows * num_cols> const &arr);

template<typename T, int num_rows, int num_cols>
Eigen::Matrix<T, num_rows, num_cols> arrToMat(std::array<T, num_rows * num_cols> const &arr)
{
    auto mat = Eigen::Matrix<T, num_rows, num_cols>{};

    for (int row_idx = 0; row_idx < num_rows; row_idx++)
        for (int col_idx = 0; col_idx < num_cols; col_idx++){
            auto const idx = static_cast<unsigned>(row_idx * num_cols + col_idx);
            mat(row_idx, col_idx) = arr[idx];
        }


    return mat;
}
}//GCT::Geom::EigenUtils namespace.
#endif //EIGEN_UTILS_H
