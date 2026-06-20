#ifndef BACK_PROJ_CUH
#define BACK_PROJ_CUH

#include "../../cpp/geom/proj_geom.h"
#include "../../cpp/geom/detector_1d.h"
#include "../utils/cuda_config.cuh"
#include "../data/recon_img.cuh"
#include "../geom/coord_trans_proj.cuh"
#include "../geom/coord_trans_img.cuh"
#include "../data/nd_array_texture.cuh"

namespace GCT::Recon::CUDA
{
namespace Detail
{
template<typename DetRow>
__global__ void backProjKern(DetRow const *dr,
                             Geom::CUDA::ProjGeomScanDev<DetRow> const *pg,
                             unsigned proj_idx,
                             cudaTextureObject_t proj_tex,
                             Geom::ReconImgGeom<2u> const *rg,
                             float *img)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;

    auto const num_cols = rg->x.num_samples;
    auto const num_rows = rg->y.num_samples;

    using namespace GCT::Geom::CUDA;

    if ((col_idx < num_cols) && (row_idx < num_rows)) {

        auto const pix_pos_wcs = pixPosWCS(rg, uint2{col_idx, row_idx});
        auto bp_weight = 1.0f;
        auto const det_coord = projMapping(pix_pos_wcs, pg, proj_idx, bp_weight);
        auto const proj_pix = DCSToPCS(det_coord, dr->sampler.sampling_width.val,
                                       pg->det_row_center[proj_idx]);

        auto const proj_val = tex1D<float>(proj_tex, proj_pix + 0.5f);
        img[row_idx * num_cols + col_idx] += (proj_val * bp_weight);
    }
}

template<typename DetRow, typename DetCol>
__global__ void backProjKern(DetRow const *dr,
                             DetCol const *dc,
                             Geom::CUDA::ProjGeomScanDev<DetRow, DetCol> const *pg,
                             unsigned proj_idx,
                             cudaTextureObject_t proj_tex,
                             Geom::ReconImgGeom<3u> const *rg,
                             float *vol)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;
    auto const slice_idx = blockIdx.z * blockDim.z + threadIdx.z;

    auto const num_cols = rg->x.num_samples;
    auto const num_rows = rg->y.num_samples;
    auto const num_slices = rg->z.num_samples;

    using namespace GCT::Geom::CUDA;

    if ((col_idx < num_cols) && (row_idx < num_rows) && (slice_idx < num_slices)) {

        auto const vox_pos_wcs = voxPosWCS(rg, uint3{col_idx, row_idx, slice_idx});
        auto bp_weight = 1.0f;
        auto const det_coords = projMapping(vox_pos_wcs, pg, proj_idx, bp_weight);
        auto const cent_pix = make_float2(pg->det_row_center[proj_idx],
                                          pg->det_col_center[proj_idx]);;
        auto const col_pix = DCSToPCS(det_coords.x, dr->sampler.sampling_width.val,
                                      cent_pix.x);
        auto const row_pix = DCSToPCS(det_coords.y, dc->sampler.sampling_width.val,
                                      cent_pix.y);
        auto const proj_val = tex2D<float>(proj_tex, col_pix + 0.5f, row_pix + 0.5f);
        auto const vox_lin_pos = slice_idx * num_rows * num_cols + row_idx * num_cols + col_idx;
        vol[vox_lin_pos] += (proj_val * bp_weight);
    }
}
}//Detail namespace.


template<typename DetRow, typename ReconImgType>
void backProjVoxDriven(Geom::CUDA::CTGeomDev<DetRow> const &cg, unsigned proj_idx,
                       Data::CUDA::NDArrayTexture<float, 1u> const &proj, ReconImgType &img)
{
    auto const config = GCT::CUDA::execConfig(img.dimensions());
    Detail::backProjKern<<<config.first, config.second>>>(cg.detGeom().detRow(),
        cg.projGeom(), proj_idx, proj.texObject(),
        img.geom(), img.dataPointer());
    GCT::CUDA::checkErrorKernel("2D backprojection");
}

template<typename DetRow, typename DetCol, typename ReconImgType>
void backProjVoxDriven(Geom::CUDA::CTGeomDev<DetRow, DetCol> const &cg, unsigned proj_idx,
                       Data::CUDA::NDArrayTexture<float, 2u> const &proj, ReconImgType &vol)
{
    auto const config = GCT::CUDA::execConfig(vol.dimensions());
    Detail::backProjKern<<<config.first, config.second>>>(cg.detGeom().detRow(),
        cg.detGeom().detCol(),
        cg.projGeom(), proj_idx, proj.texObject(),
        vol.geom(), vol.dataPointer());
    GCT::CUDA::checkErrorKernel("3D backprojection");
}
}//GCT::Recon::CUDA namespace.

#endif //BACK_PROJ_CUH
