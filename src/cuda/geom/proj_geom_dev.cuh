#ifndef GCT_PROJ_GEOM_DEV_CUH
#define GCT_PROJ_GEOM_DEV_CUH

#include "../../cpp/geom/proj_geom.h"
#include <vector>

namespace GCT::Geom::CUDA
{
// Detector-dependent projection geometry parameters of the entire scan/acquisition.
// The parameter values are stored in CUDA unified memory.
// Structure of arrays is preferred for coalesced memory access in CUDA kernels.
template<typename DetRow, typename ...DetCol>
struct ProjGeomScanDev;

template<>
struct ProjGeomScanDev<DetParallel>
{
    float *rot_ang_rad = nullptr;
    float *det_row_center = nullptr;
};

//For 1D arc detector fan-beam projection.
template<>
struct ProjGeomScanDev<DetFanArc>
{
    float *src_det_dist_mm = nullptr;
    float *src_iso_dist_mm = nullptr;
    float *det_row_center = nullptr;
    float2 *src_pos = nullptr;
    float2 *iso_ray_dir = nullptr;
    float2 *det_row_dir = nullptr;
};

//For 1D line detector fan-beam projection.
template<>
struct ProjGeomScanDev<DetFanLine>
{
    float *src_det_dist_mm = nullptr;
    float *src_iso_dist_mm = nullptr;
    float *det_row_center = nullptr;
    float2 *src_pos = nullptr;
    float2 *det_row_dir = nullptr;
    float2 *iso_ray_dir = nullptr;
};

//For 2D flat-panel detector cone-beam projections.
template<>
struct ProjGeomScanDev<DetFanLine, DetFanLine>
{
    float *src_det_dist_mm = nullptr;
    float *src_iso_dist_mm = nullptr;
    float *det_row_center = nullptr;
    float *det_col_center = nullptr;
    float3 *src_pos = nullptr;
    float3 *det_row_dir = nullptr;
    float3 *det_col_dir = nullptr;
    float3 *src_det_dir = nullptr;//Optical axis of X-ray camera.
    float4 *proj_mat_row_1 = nullptr;
    float4 *proj_mat_row_2 = nullptr;
    float4 *proj_mat_row_3 = nullptr;
};

//For 2D cylindrical detector cone-beam projections.
template<>
struct ProjGeomScanDev<DetFanArc, DetFanLine>
{
    float *src_det_dist_mm = nullptr;
    float *src_iso_dist_mm = nullptr;
    float *det_row_center = nullptr;
    float *det_col_center = nullptr;
    float3 *src_pos = nullptr;
    float3 *det_row_dir = nullptr;
    float3 *det_col_dir = nullptr;
    float3 *iso_ray_dir = nullptr;
};

//For 2D parallel projections.
template<>
struct ProjGeomScanDev<DetParallel, DetParallel>
{
    float *rot_ang_rad = nullptr;
    float *det_row_center = nullptr;
    float *det_col_center = nullptr;
};

//For 2D rebinned (axial) projections.
template<>
struct ProjGeomScanDev<DetParallel, DetFanLine>
{
    float *src_det_dist_mm = nullptr;
    float *src_iso_dist_mm = nullptr;
    float *rot_ang_rad = nullptr;
    float *det_row_center = nullptr;
    float *det_col_center = nullptr;
};
}//GCT::Geom::CUDA namespace.

#endif //GCT_PROJ_GEOM_DEV_CUH
