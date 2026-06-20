#ifndef CT_GEOM_H
#define CT_GEOM_H

#include <tuple>
#include "proj_geom.h"
#include "rotation_geom.h"
#include <utility>
#include <vector>
#include "geom_constraints.h"

namespace GCT::Geom
{
// Geometry of CT scan (system + projection geometries).
// DetRow (front view) and DetCol (side view) are the 1D detectors, and the different permutations
// of them define 2D detector geometry.
template<Valid1DDet DetRow, Valid1DDet ...DetCol>
requires ValidDetConfig<DetCol...>
struct CTGeom
{
public:
    CTGeom(std::tuple<DetRow, DetCol...> const &t_dg,
           std::vector<ProjGeom<DetRow, DetCol...>> const &t_fpg)
        :
        dg{t_dg}, fpg{t_fpg}
    {
        auto const info = getRotationGeom(t_fpg);
        if (info.has_value()) {
            rg = info.value();
        }
        else {
            throw std::runtime_error{"Invalid X-ray source trajectory.\n"};
        }
    }

    std::tuple<DetRow, DetCol...> const &detGeom() const noexcept
    {
        return dg;
    }
    std::vector<ProjGeom<DetRow, DetCol...>> const &projGeom() const noexcept
    {
        return fpg;
    }

    RotationGeom const &rotationGeom() const noexcept
    {
        return rg;
    }

protected:
    std::tuple<DetRow, DetCol...> dg;
    std::vector<ProjGeom<DetRow, DetCol...>> fpg;
    RotationGeom rg;
};
}//GCT::Geom namespace.
#endif //CT_GEOM_H
