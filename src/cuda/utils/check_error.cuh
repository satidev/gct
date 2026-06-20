#ifndef CHECK_ERROR_CUH
#define CHECK_ERROR_CUH

#include <string>

namespace GCT::CUDA
{
void checkError(cudaError_t result, std::string const &func_desc);
//After kernel call.
void checkErrorKernel(std::string const &kern_desc, bool block = false);
}//GCT::CUDA namespace.

#endif //CHECK_ERROR_CUH


