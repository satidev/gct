#ifndef RECON_IMG_H
#define RECON_IMG_H

#include "nd_array.h"
#include "../geom/recon_img_geom.h"
#include <type_traits>
#include "../geom/geom_constraints.h"

namespace GCT::Data
{
// Presence or absence of information about the detector column
// determines the dimension of the reconstructed image (2D or 3D).
template<typename ...DetCol>
constexpr size_t reconImgDim()
{
    return sizeof...(DetCol) + 2U;
}

// 2D or 3D reconstructed image.
template<typename PixType, size_t Dim> requires Geom::ValidReconImgDim<Dim>
class ReconImg: public NDArray<PixType, Dim>
{
public:
    explicit ReconImg(Geom::ReconImgGeom<Dim> const &geom);

    Geom::ReconImgGeom<Dim> const &geom() const noexcept
    {
        return geom_;
    }

private:
    Geom::ReconImgGeom<Dim> geom_;
};

template<typename PixType, size_t Dim>
requires Geom::ValidReconImgDim<Dim>
ReconImg<PixType, Dim>::ReconImg(Geom::ReconImgGeom<Dim> const &geom)
try
    :NDArray<PixType, Dim>{reconImgSize(geom), PixType{}},
     geom_{geom}
{
    if (!isSamplingValid(geom)) {
        throw std::invalid_argument{
            std::string{"Error during the allocation of the recon image.\n"} +
                std::string{
                    "The sampling width of the image in any dimension cannot be zero or negative.\n"}};
    }
}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        std::string{"Error during the allocation of the recon image.\n"} +
            std::string{
                "The number of samples in any dimension cannot be zero.\n"}};
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of the recon image.\n" + std::string{e.what()}};
}
}//GCT::Data namespace.

#endif //RECON_IMG_H
