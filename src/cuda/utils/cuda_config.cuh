#ifndef GCT_CORE_CUDA_CONFIG_CUDA_CONFIG_CUH_
#define GCT_CORE_CUDA_CONFIG_CUDA_CONFIG_CUH_

#include <array>
#include "../../cpp/data/recon_img.h"
#include "../../cpp/data/projection_stack.h"


namespace GCT::CUDA
{
//TODO:Take consider allowed maximum number of thread per block.
template<size_t Dim>
std::pair<dim3, dim3> execConfig(std::array<unsigned int, Dim> const &size);

unsigned int gridSize(unsigned int tot_size, unsigned int block_size);

}//GCT::CUDA namespace.

#endif //GCT_CORE_CUDA_CONFIG_CUDA_CONFIG_CUH_
