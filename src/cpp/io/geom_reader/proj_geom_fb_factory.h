#ifndef PROJ_GEOM_FB_FACTORY_H
#define PROJ_GEOM_FB_FACTORY_H

#include "../../geom/proj_geom.h"
#include "../parameters/proj_geom_params.h"

namespace GCT::Geom::ProjGeomFBFactory
{
// 2D fan-beam projection geometry parameters.
struct ProjGeomFanBeam
{
    Units::MilliMeters src_det_dist;
    Units::MilliMeters src_iso_dist;
    Units::DetPixels det_row_center;
    Units::Radians rot_ang;
    VecWCS<2u> src_pos;
    VecWCS<2u> det_row_dir;
    VecWCS<2u> src_det_dir;
};
ProjGeomFanBeam create(FanBeamProjGeomParams const &params, unsigned proj_idx);
ProjGeomFanBeam create(FanBeamProjGeomParams const &params, Units::Radians rot_ang);
}//GCT::Geom::ProjGeomFBFactory namespace.

#endif //PROJ_GEOM_FB_FACTORY_H
