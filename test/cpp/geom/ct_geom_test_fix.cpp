#include "ct_geom_test_fix.h"

namespace GCT::Test
{
template<>
std::tuple<Geom::DetParallel> CTGeomTestFix<Geom::DetParallel>::createDet()
{
    return std::make_tuple(Geom::DetParallel{Geom::EqDistSampler{1u, 1.0f}});
}

template<>
std::tuple<Geom::DetFanArc> CTGeomTestFix<Geom::DetFanArc>::createDet()
{
    return std::make_tuple(Geom::DetFanArc{Geom::EqAngSampler {1u, 1.0f}});
}

template<>
std::tuple<Geom::DetFanLine> CTGeomTestFix<Geom::DetFanLine>::createDet()
{
    return std::make_tuple(Geom::DetFanLine{Geom::EqDistSampler{1u, 1.0f}});
}

template<>
std::tuple<Geom::DetParallel, Geom::DetParallel> CTGeomTestFix<Geom::DetParallel, Geom::DetParallel>::createDet()
{
    return std::make_tuple(Geom::DetParallel{Geom::EqDistSampler{1u, 1.0f}},
                           Geom::DetParallel{Geom::EqDistSampler{2u, 2.0f}});
}

template<>
std::tuple<Geom::DetParallel, Geom::DetFanLine> CTGeomTestFix<Geom::DetParallel, Geom::DetFanLine>::createDet()
{
    return std::make_tuple(Geom::DetParallel{Geom::EqDistSampler{1u, 1.0f}},
                           Geom::DetFanLine{Geom::EqDistSampler{2u, 2.0f}});
}

template<>
std::tuple<Geom::DetFanArc, Geom::DetFanLine> CTGeomTestFix<Geom::DetFanArc, Geom::DetFanLine>::createDet()
{
    return std::make_tuple(Geom::DetFanArc{Geom::EqAngSampler {1u, 1.0f}},
                           Geom::DetFanLine{Geom::EqDistSampler{2u, 2.0f}});
}

template<>
std::tuple<Geom::DetFanLine, Geom::DetFanLine> CTGeomTestFix<Geom::DetFanLine, Geom::DetFanLine>::createDet()
{
    return std::make_tuple(Geom::DetFanLine{Geom::EqDistSampler{1u, 1.0f}},
                           Geom::DetFanLine{Geom::EqDistSampler{2u, 2.0f}});
}

}//GCT::Test namespace.
