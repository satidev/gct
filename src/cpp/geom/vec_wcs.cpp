#include "vec_wcs.h"

namespace GCT::Geom
{

std::ostream &operator<<(std::ostream &os, VecWCS<2u> const &vec)
{
    os << std::string{"{ " + std::to_string(vec.x.val) + ", " +
        std::to_string(vec.y.val) + " }"};
    return os;
}

std::ostream &operator<<(std::ostream &os, VecWCS<3u> const &vec)
{
    os << std::string{"{ " +  std::to_string(vec.x.val) + ", " +
        std::to_string(vec.y.val) + ", " + std::to_string(vec.z.val) +
        " }"};
    return os;
}

}//GCT::Geom namespace.
