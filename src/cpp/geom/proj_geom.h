#ifndef PROJ_GEOM_H
#define PROJ_GEOM_H

#include <array>
#include <vector>
#include <string>
#include "vec_wcs.h"
#include "detector_1d.h"
#include <algorithm>
#include <optional>
#include "utils.h"
#include "geom_constraints.h"
#include "units.h"

namespace GCT::Geom
{
// Detector-dependent projection geometry parameters.
template<Valid1DDet DetRow, Valid1DDet ...DetCol> requires ValidDetConfig<DetCol...>
struct ProjGeom;

// For 1D parallel projection.
template<>
struct ProjGeom<DetParallel>
{
    Units::Radians rot_ang;
    //Point where iso-ray meets the detector.
    Units::DetPixels det_row_center;
};

// For 1D arc-shaped detector fan-beam projection.
template<>
struct ProjGeom<DetFanArc>
{
    Units::MilliMeters src_det_dist;
    //Point where iso-ray meets the detector.
    Units::DetPixels det_row_center;
    VecWCS<2u> src_pos;
    //Direction of the tangent to the arc at the point where iso-ray meets detector
    VecWCS<2u> det_row_dir;
    VecWCS<2u> iso_ray_dir;
};

// For 1D line-shaped detector fan-beam projection.
template<>
struct ProjGeom<DetFanLine>
{
    Units::MilliMeters src_det_dist;
    Units::DetPixels det_row_center;
    VecWCS<2u> src_pos;
    VecWCS<2u> det_row_dir;
};

// For 2D parallel projections.
template<>
struct ProjGeom<DetParallel, DetParallel>
{
    Units::Radians rot_ang;
    Units::DetPixels det_row_center;
    Units::DetPixels det_col_center;
};

// For 2D rebinned (axial) projections.
template<>
struct ProjGeom<DetParallel, DetFanLine>
{
    Units::MilliMeters src_det_dist;
    Units::MilliMeters src_iso_dist;
    Units::Radians rot_ang;
    Units::DetPixels det_row_center;
    Units::DetPixels det_col_center;
};

// For 2D cylindrical detector cone-beam projections.
template<>
struct ProjGeom<DetFanArc, DetFanLine>
{
    Units::MilliMeters src_det_dist;

    //Point where iso-ray hits the detector.
    Units::DetPixels det_row_center;
    Units::DetPixels det_col_center;

    VecWCS<3u> src_pos;
    //Direction of the tangent to the arc at the point where iso-ray meets detector.
    VecWCS<3u> det_row_dir;
    VecWCS<3u> det_col_dir;
    VecWCS<3u> iso_ray_dir;
};

// For 2D flat-panel detector cone-beam projections.
template<>
struct ProjGeom<DetFanLine, DetFanLine>
{
    Units::MilliMeters src_det_dist;

    //Principal point.
    Units::DetPixels det_row_center;
    Units::DetPixels det_col_center;

    VecWCS<3u> src_pos;
    VecWCS<3u> det_row_dir;
    VecWCS<3u> det_col_dir;
};

}//GCT::Geom namespace.

#endif //PROJ_GEOM_H
