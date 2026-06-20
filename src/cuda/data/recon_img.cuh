#ifndef RECON_IMG_CUH
#define RECON_IMG_CUH

#include "../../cpp/geom/recon_img_geom.h"
#include "nd_array_dev.cuh"
#include "copy_host_dev.cuh"
#include "../../cpp/geom/vec_wcs.h"
#include "../../cpp/geom/geom_constraints.h"

namespace GCT::Data::CUDA
{
//2D or 3D reconstructed image.
template<typename PixType, size_t Dim, typename Alloc>
requires Geom::ValidReconImgDim<Dim>
class ReconImg: public NDArrayDev<PixType, Dim, Alloc>
{
public:
    explicit ReconImg(Geom::ReconImgGeom<Dim> const &geom);

    Geom::ReconImgGeom<Dim> const *geom() const noexcept
    {
        return geom_dev_.dataPointer();
    }

private:
    UnifiedMem<Geom::ReconImgGeom<Dim>> geom_dev_;
};

template<typename PixType, size_t Dim, typename DevAlloc>
requires Geom::ValidReconImgDim<Dim>
ReconImg<PixType, Dim, DevAlloc>::ReconImg(Geom::ReconImgGeom<Dim> const &geom)
try
    :Data::CUDA::NDArrayDev<PixType, Dim, DevAlloc>{reconImgSize(geom)},
     geom_dev_{1u}
{
    if (!isSamplingValid(geom)) {
        throw std::invalid_argument{
            std::string{"Error during the allocation of the recon image.\n"} +
                std::string{
                    "The sampling width of image in any dimension cannot be zero or negative.\n"}};
    }
    try {
        Data::CUDA::copyHostToDev(&geom, geom_dev_.dataPointer(), 0u, 0u, 1u);
    }
    catch (std::runtime_error const &e) {
        throw std::runtime_error{"Error during host to device transfer of recon geometry.\n" +
            std::string{e.what()}};
    }
}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        std::string{"Error during the allocation of the recon image.\n"} +
            std::string{
                "The number of samples in any dimension cannot be zero or negative.\n"}};
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of recon image.\n" + std::string{e.what()}};
}
}//GCT::Data::CUDA namespace.


#endif //RECON_IMG_CUH
