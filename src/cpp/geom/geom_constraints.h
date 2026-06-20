#ifndef GEOM_CONSTRAINTS_H
#define GEOM_CONSTRAINTS_H

#include <concepts>
#include <cstddef>
#include "detector_1d.h"

namespace GCT::Geom
{
// Currently, only uniformly sampled detectors are supported.
template<typename DetRow>
concept ValidDetSampling = requires(DetRow const &dr){
    { dr.sampler.num_samples } -> std::convertible_to<unsigned>;
    { dr.sampler.sampling_width.val } -> std::convertible_to<float>;
};

template<typename DetRow>
concept Valid1DDet = ValidDetSampling<DetRow>;

template<typename ...DetCol>
concept ValidDetConfig = (sizeof...(DetCol) < 2u);

template<size_t Dim>
concept ValidProjDim = (Dim == size_t{1u}) || (Dim == size_t{2u});

template<size_t Dim>
concept ValidReconImgDim = (Dim == size_t{2u}) || (Dim == size_t{3u});

}// GCT::Geom namespaces.

#endif //GEOM_CONSTRAINTS_H
