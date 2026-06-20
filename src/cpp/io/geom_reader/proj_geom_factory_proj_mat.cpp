#include "proj_geom_factory.h"
#include "../../geom/projection_matrix.h"
#include "../../geom/eigen_utils.h"

namespace GCT::Geom
{
template<>
template<>
ProjGeom<DetFanLine, DetFanLine> ProjGeomFactory<DetFanLine, DetFanLine>::create(
    ProjectionMatrix const &geom)
{
    auto pg = ProjGeom<DetFanLine, DetFanLine>{};
    //TODO:: Multiply with pixel size.
    pg.src_det_dist = Units::MilliMeters{geom.focalLengthDetRow()};
    pg.det_row_center = Units::DetPixels{geom.principalPointDetRow()};
    pg.det_col_center = Units::DetPixels{geom.principalPointDetCol()};

    auto const src_pos = geom.srcPos();
    pg.src_pos = EigenUtils::getVecWCS3D(src_pos);

    auto const rot_mat = geom.rotationMatrix();
    pg.det_row_dir = EigenUtils::getVecWCS3D(rot_mat.row(0));
    pg.det_col_dir = EigenUtils::getVecWCS3D(rot_mat.row(1));

    return pg;
}
}//GCT::Geom namespace.
