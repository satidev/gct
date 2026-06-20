#include "geom_math.h"
#include <Eigen/Dense>

namespace GCT::Geom::Math
{
VecWCS<3u> crossProduct(VecWCS<3u> const &v_1, VecWCS<3u> const &v_2)
{
    Eigen::Vector3f v1_e{v_1.x.val, v_1.y.val, v_1.z.val};
    Eigen::Vector3f v2_e{v_2.x.val, v_2.y.val, v_2.z.val};
    Eigen::Vector3f res = v1_e.cross(v2_e);
    return VecWCS<3u>{res(0), res(1), res(2)};
}

}//GCT::Geom::Math namespace.
