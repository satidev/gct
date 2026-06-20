#ifndef PROJ_GEOM_CB_FACTORY_H
#define PROJ_GEOM_CB_FACTORY_H

#include "../../geom/proj_geom.h"
#include "../parameters/proj_geom_params.h"

namespace GCT::Geom::ProjGeomCBFactory
{
// 3D cone-beam projection geometry parameters.
struct ProjGeomConeBeam
{
    Units::MilliMeters src_det_dist;
    Units::MilliMeters src_iso_dist; //Source-isocenter distance.
    Units::DetPixels det_row_center;// Principal point along column direction (unit pixels).
    Units::DetPixels det_col_center;// Principal point along row direction (unit pixels).*/
    Units::Radians rot_ang;// Gantry (Source-detector) rotation angle (unit radian).
    VecWCS<3u> src_pos;// Source (focal spot) position vector.
    // Basis vector of Source-Detector Coordinate System (SDCS), rows of rotation matrix
    VecWCS<3u> det_row_dir;//Unit vector along detector column.
    VecWCS<3u> det_col_dir;//Unit vector along detector row.
    VecWCS<3u> src_det_dir;//Unit normal vector to detector (from the source).
};
ProjGeomConeBeam create(ConeBeamProjGeomParams const &params, unsigned proj_idx);
ProjGeomConeBeam create(ConeBeamProjGeomParams const &params, Units::Radians rot_ang);
}//GCT::Geom::ProjGeomCBFactory namespace.

#endif //PROJ_GEOM_CB_FACTORY_H
