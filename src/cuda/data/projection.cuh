#ifndef PROJECTION_CUH
#define PROJECTION_CUH

#include <array>
#include "../../cpp/data/proj_meta_info.h"
#include "nd_array_dev.cuh"
#include "unified_mem.cuh"
#include "../../cpp/geom/geom_constraints.h"

namespace GCT::Data::CUDA
{
// 1D or 2D projection.
template<typename PixType, size_t ProjDim, typename Alloc>
requires Geom::ValidProjDim<ProjDim>
class Projection: public NDArrayDev<PixType, ProjDim, Alloc>
{
public:
    explicit Projection(Data::ProjSize<ProjDim> const &size);

    ProjSize<ProjDim> projSize() const noexcept
    {
        return proj_size_;
    }
private:
    ProjSize<ProjDim> proj_size_;
};

template<typename PixType, size_t ProjDim, typename Alloc>
requires Geom::ValidProjDim<ProjDim>
Projection<PixType, ProjDim, Alloc>::Projection(Data::ProjSize<ProjDim> const &size)
try
    :
    NDArrayDev<PixType, ProjDim, Alloc>{projSizeToArr(size)},
    proj_size_{size}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Projection size cannot be zero.\n" + std::string{e.what()}};
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of projection.\n" + std::string{e.what()}};
}

}//GCT::Data::CUDA namespace.

#endif //PROJECTION_CUH
