#ifndef SAMPLING_INFO_H
#define SAMPLING_INFO_H

#include "units.h"

namespace GCT::Geom
{
template<typename WidthUnit>
struct Sampler1D
{
    unsigned num_samples;
    WidthUnit sampling_width;
};
// Equidistant-sampling.
using EqDistSampler = Sampler1D<Units::MilliMeters>;
// Equi-angular-sampling.
using EqAngSampler = Sampler1D<Units::Radians>;

struct SamplingInfo1D
{
    unsigned num_samples;
    float sampling_width;
};
}//GCT::Geom namespace.

#endif //SAMPLING_INFO_H
