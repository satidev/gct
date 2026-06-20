#ifndef COS_WEIGHTING_CUH
#define COS_WEIGHTING_CUH

#include "../../cpp/geom/proj_geom.h"
#include "../../cpp/geom/detector_1d.h"
#include "../utils/cuda_config.cuh"
#include "../geom/coord_trans_proj.cuh"
#include "../data/projection_stack.cuh"
#include "../utils/check_error.cuh"
#include "../geom/ct_geom_dev.cuh"

namespace GCT::Recon::CUDA
{
namespace Detail
{
__device__ __host__ inline float cosWeightFanArc(float fan_ang_rad);
__device__ __host__ inline float cosWeightFanArc(float fan_ang_rad)
{
    return cosf(fan_ang_rad);
}

__device__ __host__ inline float cosWeightFanLine(float src_det_dist_mm, float det_coord_mm);
__device__ __host__ inline float cosWeightFanLine(float src_det_dist_mm, float det_coord_mm)
{
    return src_det_dist_mm / sqrtf(det_coord_mm * det_coord_mm +
        src_det_dist_mm * src_det_dist_mm);
}

// 1D parallel.
__device__ __host__ inline float cosWeight(Geom::DetParallel const *dr,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetParallel> const *fpg,
                                           unsigned proj_idx, unsigned col_idx);
__device__ __host__ inline float cosWeight(Geom::DetParallel const *dr,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetParallel> const *fpg,
                                           unsigned proj_idx, unsigned col_idx)
{
    return 1.0f;
}

// 1D arc detector.
__device__ __host__ inline float cosWeight(Geom::DetFanArc const *dr,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanArc> const *fpg,
                                           unsigned proj_idx, unsigned col_idx);
__device__ __host__ inline float cosWeight(Geom::DetFanArc const *dr,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanArc> const *fpg,
                                           unsigned proj_idx, unsigned col_idx)
{
    // From Pixel Coordinate System (PCS) to Detector Coordinate System (DCS).
    using namespace Geom::CUDA;
    auto const coord = PCSToDCS(col_idx, dr->sampler.sampling_width.val,
                                fpg->det_row_center[proj_idx]);
    return cosWeightFanArc(coord);
}

//1D line detector.
__device__ __host__ inline float cosWeight(Geom::DetFanLine const *dr,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx);
__device__ __host__ inline float cosWeight(Geom::DetFanLine const *dr,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx)
{
    // From Pixel Coordinate System (PCS) to Detector Coordinate System (DCS).
    using namespace Geom::CUDA;
    auto const coord = PCSToDCS(col_idx, dr->sampler.sampling_width.val,
                                fpg->det_row_center[proj_idx]);
    auto const sdd_mm = fpg->src_det_dist_mm[proj_idx];
    return cosWeightFanLine(sdd_mm, coord);
}

//2D parallel.
__device__ __host__ inline float cosWeight(Geom::DetParallel const *dr, Geom::DetParallel const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetParallel,
                                                                       Geom::DetParallel> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx);
__device__ __host__ inline float cosWeight(Geom::DetParallel const *dr,
                                           Geom::DetParallel const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetParallel,
                                                                       Geom::DetParallel> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx)
{
    return 1.0f;
}
__device__ __host__ inline float cosWeight(Geom::DetParallel const *dr, Geom::DetFanLine const *dc,
                                           Geom::ProjGeom<Geom::DetParallel,
                                                          Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx);
//Rebinned (cone-parallel) detector.
__device__ __host__ inline float cosWeight(Geom::DetParallel const *dr, Geom::DetFanLine const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetParallel,
                                                                       Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx)
{
    // Linear longitudinal coordinate.
    using namespace Geom::CUDA;
    auto const coord_long = PCSToDCS(row_idx, dc->sampler.sampling_width.val,
                                     fpg->det_col_center[proj_idx]);

    // Out of plane weight, cosine of cone angle (for transforming 2D fan beam recon to 3D FDK).
    auto const out_plane_w = cosWeightFanLine(fpg->src_det_dist_mm[proj_idx], coord_long);
    return out_plane_w;
}

// 2D cylindrical detector.
__device__ __host__ inline float cosWeight(Geom::DetFanArc const *dr, Geom::DetFanLine const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanArc,
                                                                       Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx);
__device__ __host__ inline float cosWeight(Geom::DetFanArc const *dr, Geom::DetFanLine const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanArc,
                                                                       Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx)
{
    // From Pixel Coordinate System (PCS) to Detector Coordinate System (DCS).
    // Axial coordinate is fan angle in radians.
    using namespace Geom::CUDA;
    auto const coord_axial = PCSToDCS(col_idx, dr->sampler.sampling_width.val,
                                      fpg->det_row_center[proj_idx]);
    auto const coord_long = PCSToDCS(row_idx, dc->sampler.sampling_width.val,
                                     fpg->det_col_center[proj_idx]);

    // In-plane cosine weight, cosine of the fan angle, due to the 2D parallel
    // to fan beam transformation (a factor of Jacobian determinant)
    auto const in_plane_w = cosWeightFanArc(coord_axial);

    // Out of plane weight, cosine of the cone angle (for transforming 2D fan beam recon to 3D FDK).
    auto const out_plane_w = cosWeightFanLine(fpg->src_det_dist_mm[proj_idx], coord_long);

    // Final cosine weight as the product of in-plane and out-of-plane weights.
    return in_plane_w * out_plane_w;
}
// 2D flat detector.
__device__ __host__ inline float cosWeight(Geom::DetFanLine const *dr, Geom::DetFanLine const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanLine,
                                                                       Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx);
__device__ __host__ inline float cosWeight(Geom::DetFanLine const *dr, Geom::DetFanLine const *dc,
                                           Geom::CUDA::ProjGeomScanDev<Geom::DetFanLine,
                                                                       Geom::DetFanLine> const *fpg,
                                           unsigned proj_idx, unsigned col_idx, unsigned row_idx)
{
    // Pixel Coordinate System (PCS) to Detector Coordinate System (DCS).
    using namespace Geom::CUDA;
    auto const coord_axial = PCSToDCS(col_idx, dr->sampler.sampling_width.val,
                                      fpg->det_row_center[proj_idx]);
    auto const coord_long = PCSToDCS(row_idx, dc->sampler.sampling_width.val,
                                     fpg->det_col_center[proj_idx]);

    // Final cosine weight as the product of in-plane and out-of-plane weights and is computed by
    // the cosine of the angle between 3D source-detector ray and the principal ray.
    auto const sdd_mm = fpg->src_det_dist_mm[proj_idx];
    return sdd_mm / sqrtf(coord_axial * coord_axial + coord_long * coord_long + sdd_mm * sdd_mm);
}

template<typename DetRow, typename DetCol>
__global__ void cosWeightKernelSingleProj(DetRow const *dr,
                                          DetCol const *dc,
                                          Geom::CUDA::ProjGeomScanDev<DetRow, DetCol> const *pg,
                                          unsigned proj_idx,
                                          float *proj)
{
    auto const num_cols = dr->sampler.num_samples;
    auto const num_rows = dc->sampler.num_samples;

    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_cols) && (row_idx < num_rows)) {
        auto const pix_idx = row_idx * num_cols + col_idx;
        proj[pix_idx] *= cosWeight(dr, dc, pg, proj_idx, col_idx, row_idx);
    }
}

template<typename DetRow, typename DetCol>
__global__ void cosWeightKernelAllProj(DetRow const *dr,
                                       DetCol const *dc,
                                       Geom::CUDA::ProjGeomScanDev<DetRow, DetCol> const *pg,
                                       unsigned num_proj, float *proj)
{
    auto const num_cols = dr->sampler.num_samples;
    auto const num_rows = dc->sampler.num_samples;

    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;
    auto const proj_idx = blockIdx.z * blockDim.z + threadIdx.z;

    if ((col_idx < num_cols) && (row_idx < num_rows) && (proj_idx < num_proj)) {
        auto const pix_idx = proj_idx * num_cols * num_rows + row_idx * num_cols + col_idx;
        proj[pix_idx] *= cosWeight(dr, dc, pg, proj_idx, col_idx, row_idx);
    }
}

template<typename DetRow>
__global__ void cosWeightKernelSingleProj(DetRow const *dr,
                                          Geom::CUDA::ProjGeomScanDev<DetRow> const *pg,
                                          unsigned proj_idx,
                                          float *proj)
{
    auto const num_cols = dr->sampler.num_samples;
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;

    if ((col_idx < num_cols)) {
        proj[col_idx] *= cosWeight(dr, pg, proj_idx, col_idx);
    }
}

template<typename DetRow>
__global__ void cosWeightKernelAllProj(DetRow const *dr,
                                       Geom::CUDA::ProjGeomScanDev<DetRow> const *pg,
                                       unsigned num_proj,
                                       float *proj)
{
    auto const num_cols = dr->sampler.num_samples;
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const proj_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_cols) && (proj_idx < num_proj)) {
        auto const pix_idx = proj_idx * num_cols + col_idx;
        proj[pix_idx] *= cosWeight(dr, pg, proj_idx, col_idx);
    }
}

}//Detail namespace.

template<typename DetRow, typename DetCol, typename ProjCUDevType>
void runCosWeighting(Geom::CUDA::CTGeomDev<DetRow, DetCol> const &cg,
                     unsigned proj_idx, ProjCUDevType &proj)
{
    auto const config = GCT::CUDA::execConfig(proj.dimensions());
    Detail::cosWeightKernelSingleProj<<<config.first, config.second>>>(
        cg.detGeom().detRow(), cg.detGeom().detCol(),
            cg.projGeom(), proj_idx, proj.dataPointer());
    GCT::CUDA::checkErrorKernel("cosine weighting");
}

template<typename DetRow, typename DetCol, typename ProjStackCUDevType>
void runCosWeighting(Geom::CUDA::CTGeomDev<DetRow, DetCol> const &cg, ProjStackCUDevType &proj)
{
    auto const config = GCT::CUDA::execConfig(proj.dimensions());
    Detail::cosWeightKernelAllProj<<<config.first, config.second>>>(
        cg.detGeom().detRow(), cg.detGeom().detCol(),
            cg.projGeom(), proj.numProj(), proj.dataPointer());
    GCT::CUDA::checkErrorKernel("cosine weighting");
}

template<typename DetRow, typename ProjCUDevType>
void runCosWeighting(Geom::CUDA::CTGeomDev<DetRow> const &cg,
                     unsigned int proj_idx, ProjCUDevType &proj)
{
    auto const config = GCT::CUDA::execConfig(proj.dimensions());
    Detail::cosWeightKernelSingleProj<<<config.first, config.second>>>(
        cg.detGeom().detRow(), cg.projGeom(), proj_idx, proj.dataPointer());
    GCT::CUDA::checkErrorKernel("cosine weighting");
}

template<typename DetRow, typename ProjStackCUDevType>
void runCosWeighting(Geom::CUDA::CTGeomDev<DetRow> const &cg, ProjStackCUDevType &proj)
{
    auto const config = GCT::CUDA::execConfig(proj.dimensions());
    Detail::cosWeightKernelAllProj<<<config.first, config.second>>>(
        cg.detGeom().detRow(), cg.projGeom(), proj.numProj(), proj.dataPointer());
    GCT::CUDA::checkErrorKernel("cosine weighting");
}

}//GCT::Recon::CUDA namespace.

#endif //COS_WEIGHTING_CUH
