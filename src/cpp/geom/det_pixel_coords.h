#ifndef DET_PIXEL_COORDS_H
#define DET_PIXEL_COORDS_H

#include "units.h"

namespace GCT::Geom
{
template<typename DetRow, typename ...DetCol>
struct DetPixelCoords;

template<typename DetRow>
struct DetPixelCoords<DetRow>
{
    Units::DetPixels row;
};

template<typename DetRow, typename DetCol>
struct DetPixelCoords<DetRow, DetCol>
{
    Units::DetPixels row;
    Units::DetPixels column;
};

}//GCT::Geom namespace.

#endif //DET_PIXEL_COORDS_H


