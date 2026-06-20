#ifndef DETECTOR_1D_H
#define DETECTOR_1D_H

#include <tuple>
#include "sampling_info.h"

namespace GCT::Geom
{
// 1D detector for parallel-beam projection.
struct DetParallel
{
    EqDistSampler sampler;
};

// 1D arc-shaped detector for fan-beam projection.
struct DetFanArc
{
    EqAngSampler sampler;
};

// 1D line-shaped detector for fan-beam projection.
struct DetFanLine
{
    EqDistSampler sampler;
};

}//GCT::Geom namespace.

#endif //DETECTOR_1D_H
