#ifndef FDK_AXIAL_CUH
#define FDK_AXIAL_CUH

#include "../../cpp/data/recon_img.h"
#include "../../cpp/data/projection_stack.h"
#include "../../cpp/geom/axial_ct_geom.h"
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

namespace GCT::Recon::CUDA
{
template<typename DetRow, typename ...DetCol>
void reconFDK(Geom::AxialCTGeom<DetRow, DetCol...> const &cg_host,
              Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj_all_host,
              Data::ReconImg<float, Data::reconImgDim<DetCol...>()> &img_host);

namespace Detail
{
// Required global memory for fast reconstruction.
template<size_t ProjDim>
size_t reqMemoryForRecon(Data::ProjectionStack<float, ProjDim> const &proj_all_host,
                         Data::ReconImg<float, ProjDim + 1u> const &img_host);

// All projections are copied to GPU memory for processing.
template<typename DetRow, typename ...DetCol>
void reconFDKHighMem(Geom::AxialCTGeom<DetRow, DetCol...> const &cg_host,
                     Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj_all_host,
                     Data::ReconImg<float, Data::reconImgDim<DetCol...>()> &img_host);

// Only one projection is copied to GPU memory for processing.
template<typename DetRow, typename ...DetCol>
void reconFDKLowMem(Geom::AxialCTGeom<DetRow, DetCol...> const &cg_host,
                    Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj_all_host,
                    Data::ReconImg<float, Data::reconImgDim<DetCol...>()> &img_host);

}//Detail namespace.

template<size_t ProjDim>
size_t Detail::reqMemoryForRecon(Data::ProjectionStack<float, ProjDim> const &proj_all_host,
                                 Data::ReconImg<float, ProjDim + 1u> const &img_host)
{
    // To copy all projections to GPU global memory.
    auto const proj_size_bytes = proj_all_host.numElements() * sizeof(float);
    // To store all image pixel values in GPU memory.
    auto const img_size_bytes = img_host.numElements() * sizeof(float);

    auto const parker_weight_bytes = Data::numProjPixels(proj_all_host.projSize()) * sizeof(float);
    auto const filt_mem_bytes = Recon::RampFilteringFFT::reqMemory(proj_all_host);

    return proj_size_bytes + img_size_bytes + parker_weight_bytes + filt_mem_bytes;
}

template<typename DetRow, typename ...DetCol>
void reconFDK(Geom::AxialCTGeom<DetRow, DetCol...> const &cg_host,
              Data::ProjectionStack<float, Data::projDim<DetCol...>()> const &proj_all_host,
              Data::ReconImg<float, Data::reconImgDim<DetCol...>()> &img_host)
{
    auto constexpr dev_idx = 0;
    auto prop = cudaDeviceProp{};
    cudaGetDeviceProperties(&prop, dev_idx);
    auto const req_mem = Detail::reqMemoryForRecon(proj_all_host, img_host);

    //If the required memory is less than 75% of total global memory,
    //enable high-memory reconstruction where all the projections are
    //copied to GPU at once and the filtering of rows is done parallel.
    //Otherwise, copy projections one-by-one and filtering will be done sequentially.
    //TODO:: Currently only high- and low-memory reconstructions are available.
    //TODO:: Facilitate block-wise reconstruction depending on the available memory.
    //TODO:: Asynchronous copy and kernel execution using multiple streams.
    if ((req_mem / static_cast<double>(prop.totalGlobalMem)) < 0.75)
        Detail::reconFDKHighMem(cg_host, proj_all_host, img_host);
    else
        Detail::reconFDKLowMem(cg_host, proj_all_host, img_host);
}

template<typename DetRow, typename ...DetCol>
void Detail::reconFDKLowMem(Geom::AxialCTGeom<DetRow, DetCol...> const &cg_host,
                            Data::ProjectionStack<float,
                                                  Data::projDim<DetCol...>()> const &proj_all_host,
                            Data::ReconImg<float, Data::reconImgDim<DetCol...>()> &img_host)
{
    auto constexpr proj_dim = Data::projDim<DetCol...>();
    auto constexpr recon_dim = Data::reconImgDim<DetCol...>();
    // Initialize volume in CUDA device.
    auto img_dev = Data::CUDA::ReconImg<float, recon_dim, Data::CUDA::DevMem<float>>{
        img_host.geom()};
    img_dev.setVal(0.0f);

    // Copy geometry parameters to CUDA device.
    auto const ct_geom_dev = Geom::CUDA::CTGeomDev<DetRow, DetCol...>{cg_host};

    // Pre-compute generalized parker weights and copy to GPU.
    auto const parker_weights = getParkerWeights(cg_host);

    // Initialize ramp filtering module.
    auto const proj_size = proj_all_host.projSize();
    auto rf = RampFilteringFFT{std::get<0>(cg_host.detGeom()), FiltParams{},
                               Data::numProjRows(proj_size)};

    auto proj_dev = Data::CUDA::Projection<float, proj_dim, Data::CUDA::DevMem<float>>{
        proj_all_host.projSize()};

    // Projection to be bound to texture memory.
    // Pre-declare to avoid recurrent allocation in projection loop.
    auto proj_tex = Data::CUDA::NDArrayTexture < float, proj_dim>{
        projSizeToArr(proj_all_host.projSize())
    };

    for (auto proj_idx = 0u; proj_idx < proj_all_host.numProj(); proj_idx++) {
        // Copy projection to CUDA device.
        auto const num_pix_proj = Data::numProjPixels(proj_all_host.projSize());
        Data::CUDA::copyHostToDev(proj_all_host.elements(), proj_dev,
                                  proj_idx * num_pix_proj, 0u, num_pix_proj);

        runCosWeighting(ct_geom_dev, proj_idx, proj_dev);
        runParkerWeighting(parker_weights, proj_idx, proj_dev);
        rf.runFiltering(proj_dev);

        // Copy to texture memory (for bilinear interpolation in CUDA device).
        Data::CUDA::copyDevToDevTexture(proj_dev, proj_tex);
        CUDA::backProjVoxDriven(ct_geom_dev, proj_idx, proj_tex, img_dev);
    }

    // Multiply image pixel values with angular increment.
    auto const rot_ang_incr = cg_host.rotationGeom().rot_ang_incr;
    if (rot_ang_incr > Geom::Units::Radians{0.0f}) {
        img_dev *= rot_ang_incr.val;
    }

    // Copy image pixel values to host.
    auto pixel_data = std::vector<float>(img_dev.numElements());
    Data::CUDA::copyDevToHost(img_dev, pixel_data);
    img_host.setData(std::move(pixel_data));
}

template<typename DetRow, typename ...DetCol>
void Detail::reconFDKHighMem(Geom::AxialCTGeom<DetRow, DetCol...> const &cg_host,
                             Data::ProjectionStack<float,
                                                   Data::projDim<DetCol...>()> const &proj_all_host,
                             Data::ReconImg<float, Data::reconImgDim<DetCol...>()> &img_host)
{
    auto constexpr proj_dim = Data::projDim<DetCol...>();
    auto constexpr recon_dim = Data::reconImgDim<DetCol...>();

    // Copy geometry parameters to CUDA device.
    auto const ct_geom_dev = Geom::CUDA::CTGeomDev<DetRow, DetCol...>{cg_host};


    // Copy all projections to GPU.
    auto proj_dev = Data::CUDA::ProjectionStack<float, proj_dim, Data::CUDA::DevMem<float>>{
        proj_all_host.projSize(),
        proj_all_host.numProj()};
    Data::CUDA::copyHostToDev(proj_all_host.elements(), proj_dev);

    { //Projection processing in a block.
        runCosWeighting(ct_geom_dev, proj_dev);
        auto const parker_weights = CUDA::getParkerWeights(cg_host);
        runParkerWeighting(parker_weights, proj_dev);
        auto const proj_size = proj_all_host.projSize();
        auto const num_rows_filt = Data::numProjRows(proj_size) * proj_all_host.numProj();
        auto rf = RampFilteringFFT{std::get<0>(cg_host.detGeom()), FiltParams{}, num_rows_filt};
        rf.runFiltering(proj_dev);
    } //Resources of the temporary objects will be deallocated.

    // Initialize volume in CUDA device.
    auto img_dev = Data::CUDA::ReconImg<float, recon_dim, Data::CUDA::DevMem<float>>{img_host.geom()};
    img_dev.setVal(0.0f);

    // Projection to be bound to texture memory for backprojection.
    // Pre-declare to avoid recurrent allocation in projection loop.
    auto proj_tex = Data::CUDA::NDArrayTexture < float, proj_dim>{
        projSizeToArr(proj_all_host.projSize())
    };
    for (auto proj_idx = 0u; proj_idx < proj_all_host.numProj(); proj_idx++) {
        auto const skip_mum_pix = proj_idx * Data::numProjPixels(proj_dev.projSize());
        Data::CUDA::copyDevToDevTexture(proj_dev, proj_tex, skip_mum_pix);
        CUDA::backProjVoxDriven(ct_geom_dev, proj_idx, proj_tex, img_dev);
    }

    // Multiply image pixel values with angular increment.
    auto const rot_ang_incr = cg_host.rotationGeom().rot_ang_incr;
    if (rot_ang_incr > Geom::Units::Radians{0.0f}) {
        img_dev *= rot_ang_incr.val;
    }

    // Copy recon image pixel values to host.
    auto pixel_data = std::vector<float>(img_dev.numElements());
    Data::CUDA::copyDevToHost(img_dev, pixel_data);
    img_host.setData(std::move(pixel_data));
}

}//GCT::Recon::CUDA namespace.


#endif //FDK_AXIAL_CUH
