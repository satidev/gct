#ifndef DET_COORDS_H
#define DET_COORDS_H

#include "detector_1d.h"
#include "units.h"

namespace GCT::Geom
{
template<typename DetRow, typename ...DetCol>
struct DetCoords;

// For 1D parallel projection.
template<>
struct DetCoords<DetParallel>
{
    Units::MilliMeters row;
};

// For 1D arc-shaped detector fan-beam projection.
template<>
struct DetCoords<DetFanArc>
{
    Units::Radians row;
};

// For 1D line-shaped detector fan-beam projection.
template<>
struct DetCoords<DetFanLine>
{
    Units::MilliMeters row;
};

// For 2D parallel projections.
template<>
struct DetCoords<DetParallel, DetParallel>
{
    Units::MilliMeters row;
    Units::MilliMeters column;
};

// For 2D rebinned (axial) projections.
template<>
struct DetCoords<DetParallel, DetFanLine>
{
    Units::MilliMeters row;
    Units::MilliMeters column;
};

// For 2D cylindrical detector cone-beam projections.
template<>
struct DetCoords<DetFanArc, DetFanLine>
{
    Units::Radians row;
    Units::MilliMeters column;
};

// For 2D flat-panel detector cone-beam projections.
template<>
struct DetCoords<DetFanLine, DetFanLine>
{
    Units::MilliMeters row;
    Units::MilliMeters column;
};

}//GCT::Geom namespace.

#endif //DET_COORDS_H
