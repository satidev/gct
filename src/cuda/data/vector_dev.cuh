#ifndef VECTOR_DEV_CUH
#define VECTOR_DEV_CUH

#include "nd_array_dev.cuh"
#include "unified_mem.cuh"
#include <array>

namespace GCT::Data::CUDA
{
template<typename ElemType, typename Alloc = DevMem<ElemType>>
class VectorDev: public CUDA::NDArrayDev<ElemType, 1u, Alloc>
{
public:
    explicit VectorDev(unsigned num_elem);
};

template<typename ElemType, typename Alloc>
VectorDev<ElemType, Alloc>::VectorDev(unsigned num_elem)
try
    :Data::CUDA::NDArrayDev<ElemType, 1u, Alloc>{std::array<unsigned, 1u>{num_elem}}
{

}
catch (std::exception const &e) {
    throw std::runtime_error{"Error during the allocation of CUDA vector.\n" +
        std::string{e.what()}};
}

}//GCT::Data::CUDA namespace.

#endif //VECTOR_DEV_CUH
