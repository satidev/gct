#ifndef FDK_SPIRAL_CUH
#define FDK_SPIRAL_CUH

#include "../../cpp/data/recon_img.h"
#include "../../cpp/data/projection_stack.h"
#include "../data/projection.cuh"
#include "../data/recon_img.cuh"
#include "cos_weighting.cuh"
#include "back_proj.cuh"
#include "../../cpp/recon/proj_filt_kernel.h"
#include "../../cpp/data/proj_meta_info.h"
#include "../../cpp/geom/recon_img_geom.h"
#include "parker_weighting.cuh"
#include "../data/projection_stack.cuh"
#include "ramp_filtering_fft.cuh"
#include "../data/dev_mem.cuh"
#include "../data/nd_array_texture.cuh"
#include "../geom/ct_geom_dev.cuh"
#include "../../cpp/io/proj_reader/projection_reader.h"
#include "../../cpp/data/proj_meta_info.h"
#include "../data/stat.cuh"
#include "../../cpp/data/stat.h"
#include "../../cpp/geom/spiral_ct_geom.h"

namespace GCT::Recon::CUDA
{
// FDK reconstruction for spiral scans (proposed by Ge Wang).
template<typename DetRow, typename ...DetCol>
void reconFDK(Geom::SpiralCTGeom<DetRow, DetCol...> const &geom_host,
              Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj_all_host,
              Data::ReconImg<float, 3u> &img_host);

template<typename DetRow, typename ...DetCol>
void reconFDK(Geom::SpiralCTGeom<DetRow, DetCol...> const &geom_host,
              Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj_all_host,
              Data::ReconImg<float, 3u> &img_host)
{
    // Copy geometry parameters to CUDA device.
    auto const geom_dev = Geom::CUDA::CTGeomDev<DetRow, DetCol...>{geom_host};

    // Initialize ramp filtering module.
    auto rf = RampFilteringFFT{std::get<0>(geom_host.detGeom()),
                               FiltParams{},
                               Data::numProjRows(proj_all_host.projSize())};

    auto constexpr proj_dim = Data::projDim<DetCol...>();
    auto proj_dev = Data::CUDA::Projection<float, proj_dim, Data::CUDA::DevMem<float>>{
        proj_all_host.projSize()};
    auto proj_tex = Data::CUDA::NDArrayTexture < float, proj_dim>{
        projSizeToArr(proj_all_host.projSize())
    };

    auto const vol_geom = img_host.geom();
    auto const first_slice_pos = Geom::leftEndPointPos(vol_geom.mid_point.z.val,
                                                       vol_geom.z.num_samples,
                                                       vol_geom.z.sampling_width.val);
    // Reconstruction slice-by-slice
    for (auto slice_idx = 0u; slice_idx < vol_geom.z.num_samples; slice_idx++) {
        auto const slice_pos = first_slice_pos + static_cast<float>(slice_idx) *
            vol_geom.z.sampling_width.val;
        std::cout << "Slice position:: " << slice_pos << std::endl;
        auto vg_slice = vol_geom;
        vg_slice.z.num_samples = 1u;
        vg_slice.mid_point.z.val = slice_pos;
        // Initialize volume in CUDA device.
        auto img_dev = Data::CUDA::ReconImg<float, 3u, Data::CUDA::DevMem<float>>{vg_slice};
        img_dev.setVal(0.0f);

        auto const [proj_idx_first, proj_idx_last] = geom_host.firstAndLastProjIndices(slice_pos);
        if ((proj_idx_first > 0u) && (proj_idx_last < proj_all_host.numProj())) {
            for (auto proj_idx = proj_idx_first; proj_idx < proj_idx_last; proj_idx++) {

                // Copy projection to GPU.
                auto const num_pix_proj = Data::numProjPixels(proj_all_host.projSize());
                Data::CUDA::copyHostToDev(proj_all_host.elements(), proj_dev,
                                          proj_idx * num_pix_proj, 0u, num_pix_proj);
                // Projection processing.
                runCosWeighting(geom_dev, proj_idx, proj_dev);
                rf.runFiltering(proj_dev);

                // Copy to texture memory (for bilinear interpolation in CUDA).
                Data::CUDA::copyDevToDevTexture(proj_dev, proj_tex);

                CUDA::backProjVoxDriven(geom_dev, proj_idx, proj_tex, img_dev);
            }
        }

        img_dev *= geom_host.rotationGeom().rot_ang_incr.val;

        // Copy reconstructed slice to host.
        auto const num_pix = img_dev.numElements();
        Data::CUDA::copyDevToHost(img_dev, img_host.elements(), 0u, slice_idx * num_pix, num_pix);
    }
}

}//GCT::Recon::CUDA namespace.

#endif //FDK_SPIRAL_CUH
