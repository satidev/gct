#ifndef GCT_CORE_CPP_RECON_PROJ_FILTER_RAMP_H_
#define GCT_CORE_CPP_RECON_PROJ_FILTER_RAMP_H_

#include "ramp_filter.h"

namespace GCT::Recon
{
template<typename DetRow>
std::vector<float> projFiltKernel(DetRow const &dr, FiltParams const &params);
}//GCT::Recon namespace.

#endif //GCT_CORE_CPP_RECON_PROJ_FILTER_RAMP_H_
