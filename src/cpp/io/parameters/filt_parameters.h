#ifndef FILT_PARAMETERS_H
#define FILT_PARAMETERS_H

namespace GCT::Recon
{
//Apodization or window function.
enum class ApodFunc
{
    NONE,
    HANN,
    HAMMING
};

struct FiltParams
{
    float cut_off_freq = 1.0f;
    ApodFunc apod_func = ApodFunc::NONE;
};
}//GCT::Recon namespace.
#endif //FILT_PARAMETERS_H
