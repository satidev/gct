#ifndef COORD_TRANS_PROJ_CUH
#define COORD_TRANS_PROJ_CUH

#include "../numeric/vec3d_math.cuh"
#include "../numeric/vec2d_math.cuh"
#include "../numeric/constants.cuh"
#include "proj_geom_dev.cuh"

// The transformations between world, detector and pixel coordinate systems.
namespace GCT::Geom::CUDA
{
// From Detector Coordinate System (DCS) to Pixel Coordinate System (PCS).
__device__ __host__ inline float DCSToPCS(float det_coord, float sampl_width, float cent_pix);

// From Pixel Coordinate (PCS) System to Detector Coordinate System (DCS).
__device__ __host__ inline float PCSToDCS(unsigned pix_coord, float sampl_width, float cent_pix);
__device__ __host__ inline float PCSToDCS(float pix_coord, float sampl_width, float cent_pix);

//The detector coordinate (axial) of parallel projection.
__device__ __host__ inline float detCoordAxialParallel(float rot_ang_rad, float x, float y);

// Projective mapping to estimate the detector coordinates of voxel/pixel centroid.
// Transformation from WCS (World Coordinate System) to DCS (Detector Coordinate System).
template<typename DetRow>
__device__ __host__ inline float projMapping(float2 const &pix_pos_wcs,
                                      CUDA::ProjGeomScanDev<DetRow> const *pg,
                                      unsigned proj_idx, float &backproj_weight);

template<typename DetRow, typename DetCol>
__device__ __host__ inline float2 projMapping(float3 const &vox_pos_wcs,
                                       CUDA::ProjGeomScanDev<DetRow, DetCol> const *pg,
                                       unsigned proj_idx, float &backproj_weight);


__device__ __host__ inline float DCSToPCS(float det_coord, float sampl_width, float cent_pix)
{
    return det_coord / sampl_width + cent_pix;
}

__device__ __host__ inline float PCSToDCS(float pix_coord, float sampl_width, float cent_pix)
{
    return (pix_coord - cent_pix) * sampl_width;
}

__device__ __host__ inline float PCSToDCS(uint pix_coord, float sampl_width, float cent_pix)
{
    return PCSToDCS(static_cast<float>(pix_coord), sampl_width, cent_pix);
}

__device__ __host__ inline float detCoordAxialParallel(float rot_ang_rad, float x, float y)
{
    // Input rotation angle is measured as the angle between the ray/rays and x-axis,
    // consistent with fan- and cone-beam projection geometries.
    // In parallel-beam geometry, the angle between x-axis and the normal to
    // the line is used as per the definition 2D Radon transform.
    using namespace Numeric::Constants::CUDA;
    auto const theta = rot_ang_rad + pi() / 2.0f;
    return x * cosf(theta) + y * sinf(theta);
}


// Parallel projection on the 1D detector.
template<>
__device__ __host__ inline float projMapping(float2 const &pix_pos_wcs,
                                      CUDA::ProjGeomScanDev<DetParallel> const *pg,
                                      unsigned proj_idx, float &backproj_weight)
{
    auto const rot_ang_rad = pg->rot_ang_rad[proj_idx];
    // The coordinate of projected point in DCS.
    auto const det_coord = detCoordAxialParallel(rot_ang_rad,
                                                 pix_pos_wcs.x,
                                                 pix_pos_wcs.y);

    backproj_weight = 1.0f;

    return det_coord;
}

// Fan-beam projection on the 1D arc detector.
template<>
__device__ __host__ inline float projMapping(float2 const &pix_pos_wcs,
                                      CUDA::ProjGeomScanDev<DetFanArc> const *pg,
                                      unsigned proj_idx, float &backproj_weight)
{
    using namespace GCT::Numeric::Vec2DMath::CUDA;

    // Basis vectors of a source-detector coordinate system.
    auto const eu = pg->det_row_dir[proj_idx];
    auto const ew = pg->iso_ray_dir[proj_idx];

    auto const src_pos_wcs = pg->src_pos[proj_idx];
    // Source-pixel vector.
    auto const src_pix_vec = pix_pos_wcs - src_pos_wcs;
    // Source-pixel distance.
    auto const src_pix_dist = norm(src_pix_vec);
    // Direction of source-pixel vector.
    auto const src_pix_dir = src_pix_vec / src_pix_dist;

    // The coordinate of projected point in DCS (fan angle in radians).
    auto const det_coord = atan2f(dot(src_pix_dir, eu), dot(src_pix_dir, ew));

    backproj_weight = pg->src_iso_dist_mm[proj_idx] / (src_pix_dist * src_pix_dist);

    return det_coord;
}

// Fan-beam projection on the 1D line detector.
template<>
__device__ __host__ inline float projMapping(float2 const &pix_pos_wcs,
                                      CUDA::ProjGeomScanDev<DetFanLine> const *pg,
                                      unsigned proj_idx, float &backproj_weight)
{
    using namespace GCT::Numeric::Vec2DMath::CUDA;

    // Basis vectors of a source-detector coordinate system.
    auto const eu = pg->det_row_dir[proj_idx];
    auto const ew = pg->iso_ray_dir[proj_idx];

    auto const src_pos_wcs = pg->src_pos[proj_idx];
    // Source-voxel vector.
    auto const src_vox_vec = pix_pos_wcs - src_pos_wcs;

    // Projected distance of source-voxel vector.
    auto const dist_w = dot(src_vox_vec, ew);

    auto const sdd = pg->src_det_dist_mm[proj_idx];

    //The coordinate of projected point in DCS.
    auto const det_coord = sdd * dot(src_vox_vec, eu) / dist_w;

    backproj_weight = sdd * pg->src_iso_dist_mm[proj_idx] / (dist_w * dist_w);

    return det_coord;
}

// Parallel projection on the 2D detector.
template<>
__device__ __host__ inline float2 projMapping(float3 const &vox_pos_wcs,
                                       CUDA::ProjGeomScanDev<DetParallel, DetParallel> const *pg,
                                       unsigned proj_idx, float &backproj_weight)
{
    auto const rot_ang_rad = pg->rot_ang_rad[proj_idx];
    // The coordinates of the projected point in DCS.
    auto const det_coord_axial = detCoordAxialParallel(rot_ang_rad,
                                                       vox_pos_wcs.x,
                                                       vox_pos_wcs.y);
    // Longitudinal coordinate is the inverse of z coordinate
    // since the z-axis is anti-parallel to unit vector along image rows.
    auto const det_coord_long = -vox_pos_wcs.z;

    // No weighting is required for parallel projections.
    backproj_weight = 1.0f;

    return make_float2(det_coord_axial, det_coord_long);
}

// Cone-parallel projection on 2D rebinned detector.
template<>
__device__ __host__ inline float2 projMapping(float3 const &vox_pos_wcs,
                                       CUDA::ProjGeomScanDev<DetParallel, DetFanLine> const *pg,
                                       unsigned proj_idx, float &backproj_weight)
{
    auto const rot_ang_rad = pg->rot_ang_rad[proj_idx];
    auto const det_coord_axial = detCoordAxialParallel(rot_ang_rad, vox_pos_wcs.x, vox_pos_wcs.y);

    auto const sdd = pg->src_det_dist_mm[proj_idx];
    auto const sid = pg->src_iso_dist_mm[proj_idx];
    auto const det_coord_long = -vox_pos_wcs.z * sdd / sid;

    backproj_weight = 1.0f;

    return make_float2(det_coord_axial, det_coord_long);
}

// Cone-beam projection on the 2D cylindrical detector.
template<>
__device__ __host__ inline float2 projMapping(float3 const &vox_pos_wcs,
                                       CUDA::ProjGeomScanDev<DetFanArc, DetFanLine> const *pg,
                                       unsigned proj_idx, float &backproj_weight)
{
    using namespace GCT::Numeric::Vec3DMath::CUDA;

    // Basis vectors of the source-detector coordinate system.
    auto const eu = pg->det_row_dir[proj_idx];
    auto const ev = pg->det_col_dir[proj_idx];
    auto const ew = pg->iso_ray_dir[proj_idx];
    auto const src_pos_wcs = pg->src_pos[proj_idx];

    // Source-voxel vector.
    auto const src_vox_vec = vox_pos_wcs - src_pos_wcs;
    // Source-voxel distance.
    auto const src_vox_dist = norm(src_vox_vec);

    // Direction of source-voxel vector.
    auto const src_vox_dir = src_vox_vec / src_vox_dist;

    // The coordinates of the projected point in DCS.
    // Axial coordinate, fan angle in radians.
    auto const det_coord_axial = atan2f(dot(src_vox_vec, eu), dot(src_vox_vec, ew));

    // Longitudinal coordinate, distance in mm.
    auto const det_coord_long = pg->src_det_dist_mm[proj_idx] * dot(src_vox_dir, ev) /
        sqrtf(1 - dot(src_vox_dir, ev) * dot(src_vox_dir, ev));

    backproj_weight = pg->src_iso_dist_mm[proj_idx] / (src_vox_dist * src_vox_dist);

    return make_float2(det_coord_axial, det_coord_long);
}

// Cone-beam projection on the 2D flat detector.
template<>
__device__ __host__ inline float2 projMapping(float3 const &vox_pos_wcs,
                                       CUDA::ProjGeomScanDev<DetFanLine, DetFanLine> const *pg,
                                       unsigned proj_idx, float &backproj_weight)
{
    using namespace GCT::Numeric::Vec3DMath::CUDA;

    // Basis vectors of the source-detector coordinate system.
    auto const eu = pg->det_row_dir[proj_idx];
    auto const ev = pg->det_col_dir[proj_idx];
    auto const ew = pg->src_det_dir[proj_idx];

    auto const src_pos_wcs = pg->src_pos[proj_idx];
    // Source-voxel vector.
    auto const src_vox_vec = vox_pos_wcs - src_pos_wcs;

    // Projected distance of source-voxel vector.
    auto const dist_w = dot(src_vox_vec, ew);

    auto const sdd = pg->src_det_dist_mm[proj_idx];
    // The coordinates of the projected point in DCS.
    auto const det_coord_axial = sdd * dot(src_vox_vec, eu) / dist_w;
    auto const det_coord_long = sdd * dot(src_vox_vec, ev) / dist_w;

    backproj_weight = sdd * pg->src_iso_dist_mm[proj_idx] / (dist_w * dist_w);

    return make_float2(det_coord_axial, det_coord_long);
}

}//GCT::Geom::CUDA namespace.
#endif //COORD_TRANS_PROJ_CUH
