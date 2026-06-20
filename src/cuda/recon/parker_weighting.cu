#include "parker_weighting.cuh"

namespace GCT::Recon::CUDA::Detail
{
__global__ void parkerWeightKern(float const *fan_angles, float const *rel_rot_angles,
                                 float det_fan_ang_rad, float rot_ang_range_rad,
                                 uint num_cols, uint num_proj, float *weight_img)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const proj_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_cols) && (proj_idx < num_proj)) {
        auto const fan_ang = fan_angles[col_idx];
        auto const rot_ang = rel_rot_angles[proj_idx];
        auto const pix_idx = proj_idx * num_cols + col_idx;
        weight_img[pix_idx] = parkerWeightFan(fan_ang, det_fan_ang_rad,
                                              rot_ang, rot_ang_range_rad);
    }
}

__global__ void parkerWeightingSingleProjKernel(float const *weight_img,
                                                unsigned num_det_cols,
                                                unsigned num_det_rows,
                                                unsigned num_proj,
                                                unsigned proj_idx,
                                                float *proj)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_det_cols) && (row_idx < num_det_rows) && (proj_idx < num_proj)) {
        auto const pix_idx = row_idx * num_det_cols + col_idx;
        proj[pix_idx] *= weight_img[proj_idx * num_det_cols + col_idx];
    }
}

__global__ void parkerWeightingAllProjKernel(float const *weight_img,
                                             unsigned num_det_cols,
                                             unsigned num_det_rows,
                                             unsigned num_proj,
                                             float *proj)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;
    auto const proj_idx = blockIdx.z * blockDim.z + threadIdx.z;

    if ((col_idx < num_det_cols) && (row_idx < num_det_rows) && (proj_idx < num_proj)) {
        auto const pix_idx = proj_idx * num_det_cols * num_det_rows +
            row_idx * num_det_cols + col_idx;
        proj[pix_idx] *= weight_img[proj_idx * num_det_cols + col_idx];
    }
}

}//GCT::Recon::CUDA::Detail namespaces.
