#ifndef ND_ARRAY_DEV_CUH
#define ND_ARRAY_DEV_CUH

#include <array>
#include "../utils/cuda_config.cuh"
#include <iostream>
#include "../utils/check_error.cuh"
#include "dev_mem.cuh"
#include "../../cpp/data/data_constraints.h"
#include "../../cpp/data/nd_array_base.h"

namespace GCT::Data::CUDA
{
// N-Dimensional array.
// Base class of image and matrix types.
// Using DevAlloc strategy, the type of memory can be selected,
//(e.g., DevMemUnified for unified memory).
template<typename ElemType, size_t Dim, typename DevAlloc = DevMem<ElemType>> requires ValidDim<Dim>
class NDArrayDev: public NDArrayBase<ElemType, Dim>
{
public:
    explicit NDArrayDev(std::array<unsigned, Dim> const &dims);

    ElemType *dataPointer() noexcept
    {
        return data_.dataPointer();
    }
    ElemType const *dataPointer() const noexcept
    {
        return data_.dataPointer();
    }

    NDArrayDev &operator*=(ElemType val);
    NDArrayDev &operator+=(NDArrayDev const &arr);
    void setVal(ElemType val);

protected:
    DevAlloc data_;
};

template<typename ElemType, size_t Dim, typename DevAlloc>
requires ValidDim<Dim>
NDArrayDev<ElemType, Dim, DevAlloc>::NDArrayDev(std::array<unsigned, Dim> const &dims)
try
    :
    NDArrayBase<ElemType, Dim>{dims},
    data_{this->numElements()}
{

}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of NDArray in CUDA.\n" + std::string{e.what()}};
}

namespace Detail
{
template<typename ElemType>
__global__ void setValKern(ElemType *arr, unsigned num_elem, ElemType val)
{
    auto const idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < num_elem) {
        arr[idx] = val;
    }
}
}//Detail namespace

template<typename ElemType, size_t Dim, typename DevAlloc>
requires ValidDim<Dim>
void NDArrayDev<ElemType, Dim, DevAlloc>::setVal(ElemType val)
{
    auto const size = std::array{this->numElements()};
    auto const config = GCT::CUDA::execConfig(size);
    Detail::setValKern<<<config.first, config.second>>>(data_.dataPointer(), size.front(), val);
    GCT::CUDA::checkErrorKernel("setting values of NDArray in CUDA device");
}

namespace Detail
{
template<typename ElemType>
__global__ void arrMulKern(ElemType *arr, unsigned num_elem, ElemType val)
{
    auto const idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < num_elem) {
        arr[idx] *= val;
    }
}
}//Detail namespace

template<typename ElemType, size_t Dim, typename DevAlloc> requires ValidDim<Dim>
NDArrayDev<ElemType, Dim, DevAlloc> &
NDArrayDev<ElemType, Dim, DevAlloc>::operator*=(ElemType val)
{
    auto const size = std::array{this->numElements()};
    auto const config = GCT::CUDA::execConfig(size);
    Detail::arrMulKern<<<config.first, config.second>>>(data_.dataPointer(), size.front(), val);
    GCT::CUDA::checkErrorKernel("scalar multiplication in CUDA device");
    return *this;
}

namespace Detail
{
template<typename ElemType>
__global__ void addKern(ElemType const *ip, unsigned num_elem,
                        ElemType *op)
{
    auto const idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < num_elem) {
        op[idx] += ip[idx];
    }
}
}//Detail namespace

template<typename ElemType, size_t Dim, typename DevAlloc> requires ValidDim<Dim>
NDArrayDev<ElemType, Dim, DevAlloc> &
NDArrayDev<ElemType, Dim, DevAlloc>::operator+=(NDArrayDev const &arr)
{
    auto const size = std::array{this->numElements()};
    auto const config = GCT::CUDA::execConfig(size);
    Detail::addKern<<<config.first, config.second>>>(arr.dataPointer(),
        size.front(), data_.dataPointer());
    GCT::CUDA::checkErrorKernel("element-wise addition in CUDA device");
    return *this;
}
}//GCT::Data::CUDA namespace.

#endif //ND_ARRAY_DEV_CUH
