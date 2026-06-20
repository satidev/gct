#ifndef GCT_GCTL_CPP_IMG_PROJ_IMG_STACK_H_
#define GCT_GCTL_CPP_IMG_PROJ_IMG_STACK_H_

#include "nd_array.h"
#include <array>
#include "projection.h"
#include "proj_meta_info.h"

namespace GCT::Data
{
// A stack of 1D or 2D projections.
template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
class ProjectionStack: public NDArray<PixType, ProjDim + 1u>
{
public:
    ProjectionStack(ProjSize<ProjDim> const &size, unsigned num_proj,
                    PixType init_val = PixType{});

    ProjSize<ProjDim> projSize() const noexcept
    {
        return proj_size_;
    }
    unsigned numProj() const noexcept
    {
        return num_proj_;
    }
    Projection<PixType, ProjDim> getProj(unsigned proj_idx) const;

    void setProj(Projection<PixType, ProjDim> const &proj, unsigned proj_idx);

private:
    std::vector<PixType> extractProjVals(unsigned proj_idx) const;
    ProjSize<ProjDim> proj_size_;
    unsigned num_proj_;
};

template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
Projection<PixType, ProjDim> ProjectionStack<PixType, ProjDim>::getProj(unsigned proj_idx) const
{
    if (proj_idx < num_proj_) {
        auto proj = Projection<PixType, ProjDim>{proj_size_};
        proj.setData(extractProjVals(proj_idx));
        return proj;
    }
    else {
        throw std::out_of_range{
            "Projection index should be less than the number of projections.\n"};
    }
}
template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
ProjectionStack<PixType, ProjDim>::ProjectionStack(ProjSize<ProjDim> const &size,
                                                   unsigned num_proj, PixType init_val)
try
    :
    NDArray<PixType, ProjDim + 1u>{projSizeToArr(size, num_proj), init_val},
    proj_size_{size}, num_proj_{num_proj}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Projections size or number of projections cannot be zero.\n" +
            std::string{e.what()}};
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of projection stack in host.\n" + std::string{e.what()}};
}

template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
std::vector<PixType> ProjectionStack<PixType, ProjDim>::extractProjVals(unsigned proj_idx) const
{
    auto const num_pix = numProjPixels(proj_size_);
    auto const skip = num_pix * proj_idx;
    auto vec = std::vector<PixType>(num_pix, PixType{});
    std::copy_n(std::cbegin(this->elems_) + skip, num_pix, std::begin(vec));
    return vec;
}

template<typename PixType, size_t ProjDim>
requires Geom::ValidProjDim<ProjDim>
void ProjectionStack<PixType, ProjDim>::setProj(Projection<PixType, ProjDim> const &proj,
                                                unsigned proj_idx)
{
    if (proj_idx < numProj()) {
        if (isEqual(this->proj_size_, proj.projSize())) {
            auto const num_pix = numProjPixels(proj_size_);
            auto const skip = num_pix * proj_idx;
            auto const &vec = proj.elements();
            std::copy_n(std::cbegin(vec), num_pix, std::begin(this->elems_) + skip);
        }
        else {
            throw std::out_of_range{
                std::string{"Error while copying a single projection to the stack of projections.\n"} +
                    std::string{"Number of columns and rows should match.\n"}};
        }
    }
    else {
        throw std::out_of_range{
            "Projection index should be less than the number of projections.\n"};
    }
}
}//GCT::Data namespace.
#endif //GCT_GCTL_CPP_IMG_PROJ_IMG_STACK_H_
