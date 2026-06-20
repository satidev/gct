#ifndef GCT_GCTL_CPP_IMG_PROJ_IMG_H_
#define GCT_GCTL_CPP_IMG_PROJ_IMG_H_

#include "nd_array.h"
#include <array>
#include "proj_meta_info.h"
#include "../geom/geom_constraints.h"

namespace GCT::Data
{
// 1D or 2D projection view/image.
template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
class Projection: public NDArray<PixType, ProjDim>
{
public:
    explicit Projection(ProjSize<ProjDim> const &size, PixType init_val = PixType{});

    ProjSize<ProjDim> projSize() const noexcept
    {
        return proj_size_;
    }

private:
    ProjSize<ProjDim> proj_size_;
};

template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
Projection<PixType, ProjDim>::Projection(ProjSize<ProjDim> const &size, PixType init_val)
try
    :
    NDArray<PixType, ProjDim>{projSizeToArr(size), init_val},
    proj_size_{size}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Projection size cannot be zero.\n" +
            std::string{e.what()}};
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during projection allocation in host.\n" + std::string{e.what()}};
}

}//GCT::Data namespace.

#endif //GCT_GCTL_CPP_IMG_PROJ_IMG_H_
