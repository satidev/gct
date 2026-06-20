#ifndef AXIAL_CT_GEOM_H
#define AXIAL_CT_GEOM_H

#include "ct_geom.h"

namespace GCT::Geom
{
template<Valid1DDet DetRow, Valid1DDet ...DetCol> requires ValidDetConfig<DetCol...>
struct AxialCTGeom: public CTGeom<DetRow, DetCol...>
{
public:
    using CTGeom<DetRow, DetCol...>::CTGeom;//Constructors are inherited.
};

}//GCT::Geom namespace.

#endif //AXIAL_CT_GEOM_H
