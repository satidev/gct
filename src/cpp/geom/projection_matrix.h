#ifndef PROJECTION_MATRIX_H
#define PROJECTION_MATRIX_H

#include <Eigen/Dense>
#include <Eigen/src/Core/Matrix.h>
#include <array>
#include "eigen_utils.h"
#include "proj_geom.h"
#include "detector_1d.h"


namespace GCT::Geom
{
//Compose and decompose projection matrix.
class ProjectionMatrix
{
public:
    explicit ProjectionMatrix(ProjGeom<DetFanLine, DetFanLine> const &pg,
                              float pix_width_mm, float pix_height_mm);

    explicit ProjectionMatrix(Eigen::Matrix<double, 3, 4> const &mat);
    explicit ProjectionMatrix(std::array<double, 12U> const &mat)
        :
        ProjectionMatrix{EigenUtils::arrToMat<double, 3, 4>(mat)}
    {}

    Eigen::Matrix<float, 3, 4> matrix() const noexcept
    {
        return mat_;
    }

    float principalPointDetRow() const noexcept
    {
        return prin_point_det_row_;
    }
    float principalPointDetCol() const noexcept
    {
        return prin_point_det_col_;
    }
    //Focal length or source-detector distance (measured in number of pixels of detector row).
    float focalLengthDetRow() const noexcept
    {
        return focal_len_det_row_;
    }
    Eigen::Vector<float, 3> srcPos() const noexcept
    {
        return src_pos_;
    }
    Eigen::Matrix<float, 3, 3> rotationMatrix() const noexcept
    {
        return rot_mat_;
    }
    Eigen::Matrix<float, 3, 3> intrinsicMatrix() const noexcept
    {
        return intrinsic_mat_;
    }

private:
    static void decompose(Eigen::Matrix<double, 3, 4> const &proj_mat,
                          Eigen::Matrix3d &intrinsic_mat,
                          Eigen::Matrix3d &rot_mat,
                          Eigen::Vector3d &src_pos);

    void setParams(Eigen::Matrix<double, 3, 4> const &proj_mat,
                   Eigen::Matrix3d const &intrinsic_mat,
                   Eigen::Matrix3d const &rot_mat,
                   Eigen::Vector3d const &src_pos);

    Eigen::Matrix<float, 3, 4> mat_;
    Eigen::Matrix<float, 3, 3> intrinsic_mat_;
    Eigen::Matrix<float, 3, 3> rot_mat_;
    float prin_point_det_row_;// Principal point of each detector row (unit pixels)
    float prin_point_det_col_;// Principal point of each detector column (unit pixels).
    float focal_len_det_row_;
    Eigen::Vector<float, 3> src_pos_;// Source or camera center position in 3D.
};
}//GCT::Geom namespace.
#endif //PROJECTION_MATRIX_H
