#ifndef PROJ_GEOM_FACTORY_H
#define PROJ_GEOM_FACTORY_H

#include "../../geom/proj_geom.h"

namespace GCT::Geom
{
template<typename DetRow, typename ...DetCol>
struct ProjGeomFactory
{
template<typename GeomType>
static ProjGeom<DetRow, DetCol...> create(GeomType const &geom, unsigned proj_idx);

template<typename GeomType>
static ProjGeom<DetRow, DetCol...> create(GeomType const &geom);
};

}//GCT::Geom namespace.

#endif //PROJ_GEOM_FACTORY_H
