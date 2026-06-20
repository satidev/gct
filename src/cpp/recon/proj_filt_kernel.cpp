#include "proj_filt_kernel.h"

namespace GCT::Recon
{
template<>
std::vector<float> projFiltKernel(GCT::Geom::DetParallel const &dr,
                                  FiltParams const &params)
{
    return RampFilter::kernel(dr.sampler, params);
}

template<>
std::vector<float> projFiltKernel(GCT::Geom::DetFanLine const &dr,
                                  FiltParams const &params)
{
    return RampFilter::kernel(dr.sampler, params);
}

template<>
std::vector<float> projFiltKernel(GCT::Geom::DetFanArc const &dr,
                                  FiltParams const &params)
{
    auto ramp_filt = RampFilter::kernel(dr.sampler, params);

    auto const filt_len = ramp_filt.size();
    auto sample_pos = std::vector<int>(filt_len);
    std::iota(begin(sample_pos), end(sample_pos), -(static_cast<int>(filt_len) / 2));

    // Sample position/fan-angle dependent weighting.
    auto const fan_ang_incr_rad = static_cast<double>(dr.sampler.sampling_width.val);
    auto const weighting = [fan_ang_incr_rad](int n, float par_val)
    {
        auto const beta = static_cast<double>(n) * fan_ang_incr_rad;
        auto const weight = std::pow((beta / sin(beta)), 2);
        return static_cast<float>(weight * par_val);
    };

    auto ramp_filt_weighted = std::vector<float>{};
    std::transform(std::cbegin(sample_pos), std::cend(sample_pos), std::cbegin(ramp_filt),
                   std::back_inserter(ramp_filt_weighted), weighting);
    // If n is 0.
    ramp_filt_weighted[filt_len / 2] = ramp_filt[filt_len / 2];

    return ramp_filt_weighted;
}

}// GCT::Recon namespace.


