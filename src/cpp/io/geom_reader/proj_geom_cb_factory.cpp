#include "proj_geom_cb_factory.h"
#include "../../geom/eigen_utils.h"
#include <eigen/Eigen/Geometry>

namespace GCT::Geom::ProjGeomCBFactory
{
ProjGeomConeBeam create(ConeBeamProjGeomParams const &params, unsigned proj_idx)
{
    auto const rot_ang_rad = params.rot.init_ang +
        params.rot.ang_incr * static_cast<float>(proj_idx);
    return create(params, rot_ang_rad);
}

ProjGeomConeBeam create(ConeBeamProjGeomParams const &params, Units::Radians rot_ang)
{
    auto pg = ProjGeomConeBeam{};
    pg.src_det_dist = params.src_det_dist;
    pg.src_iso_dist = params.src_iso_dist;
    pg.det_row_center = params.det_row_center;
    pg.det_col_center = params.det_col_center;
    pg.rot_ang = rot_ang;

    using namespace Eigen;
    auto rot_mat_z = Matrix3f{};
    rot_mat_z = AngleAxisf(static_cast<float>(params.rot.dir) * rot_ang.val, Vector3f::UnitZ());

    //Basis vectors for rotation angle 0 (implicit rotation matrix), stacked row-by-row.
    auto rot_mat_implicit = Matrix3f{};
    rot_mat_implicit << 0.0, 1.0, 0.0,//u axis is parallel to y-axis of WCS.
        0.0, 0.0, -1.0,//v axis is anti-parallel to z axis.
        -1.0, 0.0, 0.0;//w axis is anti-parallel to x axis.

    //Rotated (final) basis vectors.
    auto const rot_mat = rot_mat_implicit * rot_mat_z.transpose();

    //Rows of the rotation matrix are the basis vectors of SDCS.
    pg.det_row_dir = EigenUtils::getVecWCS3D(rot_mat.row(0));
    pg.det_col_dir = EigenUtils::getVecWCS3D(rot_mat.row(1));
    pg.src_det_dir = EigenUtils::getVecWCS3D(rot_mat.row(2));

    //Compute source position after rotation.
    auto const src_pos_init = Vector3f{params.src_iso_dist.val, 0.0f,
                                       -params.rot.hel_pitch * rot_ang.val};
    auto const src_pos_rot = rot_mat_z * src_pos_init;
    pg.src_pos = EigenUtils::getVecWCS3D(src_pos_rot);

    return pg;
}

}//GCT::Geom::ProjGeomCBFactory namespace.

