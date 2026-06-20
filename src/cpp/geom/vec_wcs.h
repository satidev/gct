#ifndef GCT_WCS_H
#define GCT_WCS_H

#include <cstddef>
#include <ostream>
#include "units.h"

namespace GCT::Geom
{
// 2D/3D vector to represent points and directions in World Coordinate System (WCS).
template<size_t Dim>
struct VecWCS;

template<>
struct VecWCS<2u>
{
    Units::MilliMeters x, y;
};

template<>
struct VecWCS<3u>
{
    Units::MilliMeters x, y, z;
};

std::ostream &operator<<(std::ostream &os, VecWCS<2u> const &vec);
std::ostream &operator<<(std::ostream &os, VecWCS<3u> const &vec);

}//GCT::Geom namespace.

#endif //GCT_WCS_H
