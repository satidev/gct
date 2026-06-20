#include "eigen_utils.h"

namespace GCT::Geom::EigenUtils
{
Geom::VecWCS<3u> getVecWCS3D(Eigen::Vector3f const &vec)
{
    return Geom::VecWCS<3u>{vec(0u), vec(1u), vec(2u)};
}

}//GCT::Geom::EigenUtils namespace.
