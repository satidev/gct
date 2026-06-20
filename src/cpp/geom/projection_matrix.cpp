#include "projection_matrix.h"
#include "geom_math.h"

GCT::Geom::ProjectionMatrix::ProjectionMatrix(Eigen::Matrix<double, 3, 4> const &mat)
{
    Eigen::Matrix3d intr_mat_d;
    Eigen::Matrix3d rot_mat_d;
    Eigen::Vector3d src_pos_d;
    decompose(mat, intr_mat_d, rot_mat_d, src_pos_d);
    setParams(mat, intr_mat_d, rot_mat_d, src_pos_d);
}

void GCT::Geom::ProjectionMatrix::decompose(Eigen::Matrix<double, 3, 4> const &proj_mat,
                                            Eigen::Matrix3d &intrinsic_mat,
                                            Eigen::Matrix3d &rot_mat,
                                            Eigen::Vector3d &src_pos)
{

    Eigen::Matrix3d const invM = proj_mat.block(0, 0, 3, 3).inverse();

    //Perform QR decomposition.
    Eigen::HouseholderQR<Eigen::Matrix3<double>> const qr{invM};

    rot_mat = qr.householderQ();//Orthogonal matrix

    intrinsic_mat = qr.matrixQR();//Upper triangular matrix
    intrinsic_mat(1, 0) = 0.0;
    intrinsic_mat(2, 1) = 0.0;
    intrinsic_mat(2, 0) = 0.0;

    rot_mat.transposeInPlace();
    intrinsic_mat = intrinsic_mat.inverse().eval();

    //Make the decomposition unique by requiring that K has positive diagonal entries.
    //Achieved by multiplication with a correction matrix.
    Eigen::Matrix3<double> correction = Eigen::Matrix3<double>::Identity();
    correction(0, 0) = std::copysign(1.0, intrinsic_mat(0, 0));
    correction(1, 1) = std::copysign(1.0, intrinsic_mat(1, 1));
    correction(2, 2) = std::copysign(1.0, intrinsic_mat(2, 2));

    //Apply corrections.
    intrinsic_mat = intrinsic_mat * correction;
    rot_mat = correction * rot_mat;

    //Normalize calibration matrix.
    intrinsic_mat = intrinsic_mat / intrinsic_mat(2, 2);

    //3D Position vector of X-ray source (focal spot).
    src_pos = -invM * proj_mat.col(3);
}
void GCT::Geom::ProjectionMatrix::setParams(Eigen::Matrix<double, 3, 4> const &proj_mat,
                                            Eigen::Matrix3d const &intrinsic_mat,
                                            Eigen::Matrix3d const &rot_mat,
                                            Eigen::Vector3d const &src_pos)
{
    mat_ = proj_mat.cast<float>();
    intrinsic_mat_ = intrinsic_mat.cast<float>();
    rot_mat_ = rot_mat.cast<float>();
    src_pos_ = src_pos.cast<float>();

    //Extract parameters from the intrinsic matrix.
    prin_point_det_row_ = intrinsic_mat_(0, 2);
    prin_point_det_col_ = intrinsic_mat_(1, 2);
    focal_len_det_row_ = intrinsic_mat_(0, 0);
}
GCT::Geom::ProjectionMatrix::ProjectionMatrix(ProjGeom<DetFanLine, DetFanLine> const &pg,
                                              float pix_width_mm,
                                              float pix_height_mm)
{
    focal_len_det_row_ = pg.src_det_dist.val / pix_width_mm;
    prin_point_det_row_ = pg.det_row_center.val;

    auto const focal_len_det_col = pg.src_det_dist.val / pix_height_mm;
    prin_point_det_col_ = pg.det_col_center.val;

    //Compose intrinsic matrix.
    intrinsic_mat_ << focal_len_det_row_, 0.0f, prin_point_det_row_,
        0.0f, focal_len_det_col, prin_point_det_col_,
        0.0f, 0.0f, 1.0f;

    auto const row_dir = pg.det_row_dir;
    auto const col_dir = pg.det_col_dir;
    auto const src_det_dir = Geom::Math::crossProduct(pg.det_row_dir, pg.det_col_dir);
    rot_mat_ << row_dir.x.val, row_dir.y.val, row_dir.z.val,
        col_dir.x.val, col_dir.y.val, col_dir.z.val,
        src_det_dir.x.val, src_det_dir.y.val, src_det_dir.z.val;

    src_pos_ << pg.src_pos.x.val, pg.src_pos.y.val, pg.src_pos.z.val;

    Eigen::Matrix<float, 3, 4> extrinsic_mat;
    extrinsic_mat.block<3, 3>(0, 0) = rot_mat_;
    extrinsic_mat.block<3, 1>(0, 3) = -rot_mat_ * src_pos_;

    mat_ = intrinsic_mat_ * extrinsic_mat;
    mat_ = mat_ / mat_(2, 3);
}




