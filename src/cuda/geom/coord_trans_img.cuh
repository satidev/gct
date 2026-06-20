#ifndef COORD_TRANS_IMG_CUH
#define COORD_TRANS_IMG_CUH

#include "../data/recon_img.cuh"

// A collection of functions for the transformations between image and world coordinate systems.
namespace GCT::Geom::CUDA
{
// Computes the position of the left end point of the number line.
__device__ __host__ inline float leftEndPointPos(float mid_point, unsigned num_samples, float sampling_width);

// Computes the position vector of the centroid of the first pixel (0, 0).
__device__ __host__ inline float2 firstPixPos(Geom::ReconImgGeom<2u> const *rg);

// Computes the position vector of the centroid of the first voxel (0, 0, 0).
__device__ __host__ inline float3 firstVoxPos(Geom::ReconImgGeom<3u> const *rg);

// Affine function.
__device__ __host__ inline float affineFunc(float trans, unsigned idx, float scale);

// Computes the position vector of voxel's centroid in 3D World Coordinate System (WCS).
__device__ __host__ inline float3 voxPosWCS(Geom::ReconImgGeom<3u> const *rg, uint3 vox_idx);

// Computes the position vector of pixel's centroid in 2D World Coordinate System (WCS).
__device__ __host__ inline float2 pixPosWCS(Geom::ReconImgGeom<2u> const *rg, uint2 vox_idx);


__device__ __host__ inline float leftEndPointPos(float mid_point, unsigned num_samples, float sampling_width)
{
    return mid_point - (static_cast<float>(num_samples - 1) * sampling_width / 2.0f);
}


__device__ __host__ inline float3 firstVoxPos(Geom::ReconImgGeom<3u> const *rg)
{
    return make_float3(leftEndPointPos(rg->mid_point.x.val, rg->x.num_samples, rg->x.sampling_width.val),
                       leftEndPointPos(rg->mid_point.y.val, rg->y.num_samples, rg->y.sampling_width.val),
                       leftEndPointPos(rg->mid_point.z.val, rg->z.num_samples, rg->z.sampling_width.val));
}

__device__ __host__ inline float2 firstPixPos(Geom::ReconImgGeom<2u> const *rg)
{
    return make_float2(leftEndPointPos(rg->mid_point.x.val, rg->x.num_samples, rg->x.sampling_width.val),
                       leftEndPointPos(rg->mid_point.y.val, rg->y.num_samples, rg->y.sampling_width.val));
}

__device__ __host__ inline float affineFunc(float trans, unsigned idx, float scale)
{
    return trans + static_cast<float>(idx) * scale;
}

__device__ __host__ inline float3 voxPosWCS(Geom::ReconImgGeom<3u> const *rg, uint3 vox_idx)
{
    auto const first_vox = firstVoxPos(rg);
    return make_float3(affineFunc(first_vox.x, vox_idx.x, rg->x.sampling_width.val),
                       affineFunc(first_vox.y, vox_idx.y, rg->y.sampling_width.val),
                       affineFunc(first_vox.z, vox_idx.z, rg->z.sampling_width.val));
}

__device__ __host__ inline float2 pixPosWCS(Geom::ReconImgGeom<2u> const *rg, uint2 vox_idx)
{
    auto const first_pix = firstPixPos(rg);
    return make_float2(affineFunc(first_pix.x, vox_idx.x, rg->x.sampling_width.val),
                       affineFunc(first_pix.y, vox_idx.y, rg->y.sampling_width.val));
}

}//GCT::Geom::CUDA namespace.

#endif //COORD_TRANS_IMG_CUH
