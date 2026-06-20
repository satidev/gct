#ifndef GEOM_MATH_H
#define GEOM_MATH_H

#include "vec_wcs.h"
#include <cmath>

namespace GCT::Geom::Math
{
constexpr float norm(Geom::VecWCS<2u> const &v)
{
    return std::sqrt(v.x.val * v.x.val + v.y.val * v.y.val);
}
constexpr float norm(Geom::VecWCS<3u> const &v)
{
    return std::sqrt(v.x.val * v.x.val + v.y.val * v.y.val + v.z.val * v.z.val);
}

constexpr VecWCS<2u> rot90(VecWCS<2u> const &v)
{
    return VecWCS<2u>{-v.y.val, v.x.val};
}

VecWCS<3u> crossProduct(VecWCS<3u> const &v_1, VecWCS<3u> const &v_2);

}//GCT::Geom::Math namespace.

#endif //GEOM_MATH_H
