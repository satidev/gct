#ifndef GCT_CORE_CPP_IMG_PROC_RAMP_FILTER_H_
#define GCT_CORE_CPP_IMG_PROC_RAMP_FILTER_H_

#include "../io/parameters/filt_parameters.h"
#include <vector>
#include <cmath>
#include "../geom/detector_1d.h"
#include <algorithm>
#include <numeric>
#include <numbers>

namespace GCT::Recon
{
// Discrete ramp filter.
struct RampFilter
{
public:
    template<typename Sampler>
    static std::vector<float> kernel(Sampler const &si, FiltParams const &params);

private:
    static double M(double t)
    {
        return (t == 0.0) ? 0.5 : (std::sin(t) / t + (std::cos(t) - 1.0) / (t * t));
    }
    static double apodFact(ApodFunc func);
};

template<typename Sampler>
std::vector<float> RampFilter::kernel(Sampler const &si, FiltParams const &params)

{
    auto const apod_fact = apodFact(params.apod_func);
    auto const cut_off_freq = static_cast<double>(params.cut_off_freq);

    auto const filt_len = si.num_samples;
    auto sampl_pos = std::vector<int>(filt_len);
    std::iota(std::begin(sampl_pos), std::end(sampl_pos), -(static_cast<int>(filt_len) / 2));

    auto const sw = static_cast<double>(si.sampling_width.val);
    auto constexpr pi = std::numbers::pi;
    auto const gen_filt_coeff = [sw, apod_fact, cut_off_freq](int n)
    {
        auto const n_d = static_cast<double>(n);
        auto const filt_coeff = ((cut_off_freq * cut_off_freq) / (2.0 * sw)) *
            (apod_fact * M(cut_off_freq * pi * n_d) +
                ((1 - apod_fact) / 2.0) * M(pi * cut_off_freq * n_d + pi) +
                ((1 - apod_fact) / 2.0) * M(pi * cut_off_freq * n_d - pi));
        return static_cast<float>(filt_coeff);
    };

    auto ramp_filt = std::vector<float>{};
    std::transform(std::cbegin(sampl_pos), std::cend(sampl_pos),
                   std::back_inserter(ramp_filt), gen_filt_coeff);
    return ramp_filt;
}

}//GCT::Recon namespace.
#endif //GCT_CORE_CPP_IMG_PROC_RAMP_FILTER_H_
