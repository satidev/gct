#ifndef PROJECTION_STACK_CUH
#define PROJECTION_STACK_CUH

#include <array>
#include "../../cpp/data/proj_meta_info.h"
#include "nd_array_dev.cuh"

namespace GCT::Data::CUDA
{
// A stack or collection of 1D or 2D projections.
template<typename PixType, size_t ProjDim, typename Alloc>
requires Geom::ValidProjDim<ProjDim>
class ProjectionStack: public NDArrayDev<PixType, ProjDim + 1u, Alloc>
{
public:
    ProjectionStack(Data::ProjSize<ProjDim> const &size, unsigned num_proj);

    ProjSize<ProjDim> projSize() const noexcept
    {
        return proj_size_;
    }
    unsigned numProj() const noexcept
    {
        return num_proj_;
    }

private:
    ProjSize<ProjDim> proj_size_;
    unsigned num_proj_;
};

template<typename PixType, size_t ProjDim, typename Alloc>
requires Geom::ValidProjDim<ProjDim>
ProjectionStack<PixType, ProjDim, Alloc>::ProjectionStack(
    Data::ProjSize<ProjDim> const &size, unsigned num_proj)
try
    :
    NDArrayDev<PixType, ProjDim + 1u, Alloc>{projSizeToArr(size, num_proj)},
    proj_size_{size}, num_proj_{num_proj}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Projection size cannot be zero.\n" + std::string{e.what()}};
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of projection stack.\n" + std::string{e.what()}};
}
}//GCT::Data::CUDA namespace.


#endif //PROJECTION_STACK_CUH
