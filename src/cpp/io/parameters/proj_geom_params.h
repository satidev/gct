#ifndef GCT_ACQ_GEOM_PARAMS_H
#define GCT_ACQ_GEOM_PARAMS_H

#include "../../geom/proj_geom.h"
#include "../../geom/rotation_geom.h"
#include "../../geom/units.h"

namespace GCT::Geom
{
// World Coordinate System.
enum class WCS
{
    DCM//DICOM coordinate system.
};

struct RotationParams
{
    RotDir dir = RotDir::CW;
    Units::Radians init_ang;
    Units::Radians ang_incr;
    unsigned num_proj;
    float hel_pitch = 0.0f;
};

//Acquisition geometry parameters for a stable CT system with a perfect circular or spiral
//source trajectory.
struct Parallel1DProjGeomParams
{
    Units::DetPixels det_row_center;
    WCS wcs = WCS::DCM;
    RotationParams rot;
};

struct FanBeamProjGeomParams
{
    Units::DetPixels det_row_center;
    WCS wcs = WCS::DCM;
    RotationParams rot;
    Units::MilliMeters src_det_dist;
    Units::MilliMeters src_iso_dist;
};

struct Parallel2DProjGeomParams
{
    Units::DetPixels det_row_center;
    WCS wcs = WCS::DCM;
    RotationParams rot;
    Units::DetPixels det_col_center;
};

struct ConeBeamProjGeomParams
{
    Units::DetPixels det_row_center;
    WCS wcs = WCS::DCM;
    RotationParams rot;
    Units::MilliMeters src_det_dist;
    Units::MilliMeters src_iso_dist;
    Units::DetPixels det_col_center;
};

}//GCT::Geom namespace.

#endif //GCT_ACQ_GEOM_PARAMS_H
