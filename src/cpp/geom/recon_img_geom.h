#ifndef RECON_IMG_GEOM_H
#define RECON_IMG_GEOM_H

#include "vec_wcs.h"
#include "sampling_info.h"
#include <array>
#include <cstdio>
#include "geom_constraints.h"

namespace GCT::Geom
{
template<size_t Dim> requires ValidReconImgDim<Dim>
struct ReconImgGeom;

template<>
struct ReconImgGeom<2u>
{
    EqDistSampler x;// Column.
    EqDistSampler y;// Row.
    VecWCS<2u> mid_point;// Reconstruction center.
};

template<>
struct ReconImgGeom<3u>
{
    EqDistSampler x;// Column.
    EqDistSampler y;// Row.
    EqDistSampler z;// Slice.
    VecWCS<3u> mid_point;//Reconstruction center.
};

template<size_t Dim>
constexpr inline std::array<unsigned, Dim> reconImgSize(ReconImgGeom<Dim> const &rg);

template<size_t Dim>
constexpr inline bool isSamplingValid(ReconImgGeom<Dim> const &rg);

// Computes the first pixel/voxel position.
template<size_t Dim>
constexpr inline VecWCS<Dim> firstVoxPos(ReconImgGeom<Dim> const &rg);

// Computes the position of the left end point of the number line.
constexpr inline float leftEndPointPos(float mid_point, unsigned num_samples, float sampling_width);

template<>
constexpr inline std::array<unsigned, 2u> reconImgSize(ReconImgGeom<2u> const &rg)
{
    return std::array{rg.x.num_samples, rg.y.num_samples};
}

template<>
constexpr inline std::array<unsigned, 3u> reconImgSize(ReconImgGeom<3u> const &rg)
{
    return std::array{rg.x.num_samples, rg.y.num_samples, rg.z.num_samples};
}

template<>
constexpr inline bool isSamplingValid(ReconImgGeom<2u> const &rg)
{
    return (rg.x.num_samples > 0) && (rg.y.num_samples > 0) &&
        (rg.x.sampling_width.val > 0.0f) && (rg.y.sampling_width.val > 0.0f);
}

template<>
constexpr inline bool isSamplingValid(ReconImgGeom<3u> const &rg)
{
    return (rg.x.num_samples > 0) && (rg.y.num_samples > 0) && (rg.z.num_samples > 0) &&
        (rg.x.sampling_width.val > 0.0f) && (rg.y.sampling_width.val > 0.0f)
        && (rg.z.sampling_width.val > 0.0f);
}

template<>
constexpr inline VecWCS<2u> firstVoxPos(ReconImgGeom<2u> const &rg)
{
    return VecWCS<2u>{leftEndPointPos(rg.mid_point.x.val, rg.x.num_samples, rg.x.sampling_width.val),
                      leftEndPointPos(rg.mid_point.y.val, rg.y.num_samples, rg.y.sampling_width.val)};
}

template<>
constexpr inline VecWCS<3u> firstVoxPos(ReconImgGeom<3u> const &rg)
{
    return VecWCS<3u>{leftEndPointPos(rg.mid_point.x.val, rg.x.num_samples, rg.x.sampling_width.val),
                      leftEndPointPos(rg.mid_point.y.val, rg.y.num_samples, rg.y.sampling_width.val),
                      leftEndPointPos(rg.mid_point.z.val, rg.z.num_samples, rg.z.sampling_width.val)};
}

constexpr inline float leftEndPointPos(float mid_point, unsigned num_samples, float sampling_width)
{
    return mid_point - ((static_cast<float>(num_samples) - 1.0f) * sampling_width / 2.0f);
}

}//GCT::Geom namespace.

#endif //RECON_IMG_GEOM_H
