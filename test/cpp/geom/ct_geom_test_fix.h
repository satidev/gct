#ifndef CT_GEOM_TEST_FIX_H
#define CT_GEOM_TEST_FIX_H

#include "proj_geom_test_fix.h"
#include "../../../src/cpp/geom/detector_1d.h"
#include <tuple>
#include "../../../src/cpp/geom/ct_geom.h"

namespace GCT::Test
{
template<typename DetRow, typename ...DetCol>
struct CTGeomTestFix
{
public:
    CTGeomTestFix()
        : cg{createDet(), ProjGeomTestFix<DetRow, DetCol...>{}.projGeom()}
    {

    }
    Geom::CTGeom<DetRow, DetCol...> const &ctGeom() const noexcept{
        return cg;
    }

protected:
    Geom::CTGeom<DetRow, DetCol...> cg;

private:
    Geom::SamplingInfo1D detRow()
    {
        return Geom::SamplingInfo1D{1u, 1.0f};
    }
    Geom::SamplingInfo1D detCol()
    {
        return Geom::SamplingInfo1D{2u, 2.0f};
    }
    std::tuple<DetRow, DetCol...> createDet();
};

}

#endif //CT_GEOM_TEST_FIX_H
