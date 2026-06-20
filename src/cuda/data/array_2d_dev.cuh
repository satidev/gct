#ifndef ARRAY_2D_DEV_CUH
#define ARRAY_2D_DEV_CUH

#include "nd_array_dev.cuh"
#include "unified_mem.cuh"
#include <array>

namespace GCT::Data::CUDA
{
template<typename ElemType, typename DevAlloc = DevMem<ElemType>>
class Array2DDev: public NDArrayDev<ElemType, 2u, DevAlloc>
{
public:
    Array2DDev(unsigned num_cols, unsigned num_rows);

    unsigned numCols() const noexcept
    {
        return this->dimensions().at(0u);
    }
    unsigned numRows() const noexcept
    {
        return this->dimensions().at(1u);
    }
};

template<typename ElemType, typename DevAlloc>
Array2DDev<ElemType, DevAlloc>::Array2DDev(unsigned num_cols, unsigned num_rows)
try
    :Data::CUDA::NDArrayDev<ElemType, 2u, DevAlloc>{std::array<unsigned, 2u>{num_cols, num_rows}}
{

}
catch (std::exception const &e) {
    throw std::runtime_error{"Error during the allocation of 2D array.\n" +
        std::string{e.what()}};

}

}//GCT::Data::CUDA namespace.


#endif //ARRAY_2D_DEV_CUH
