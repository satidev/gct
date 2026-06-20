#include "ramp_filter.h"

namespace GCT::Recon
{
double RampFilter::apodFact(ApodFunc func)
{
    auto window_fact = 1.0;
    switch (func) {
        case ApodFunc::HANN:
            window_fact = 0.5;
            break;
        case ApodFunc::HAMMING:
            window_fact = 0.54;
            break;
        case ApodFunc::NONE:
            window_fact = 1.0;
            break;
    }
    return window_fact;
}
}//GCT::Recon namespace.

