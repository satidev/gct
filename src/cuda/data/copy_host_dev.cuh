#ifndef COPY_HOST_DEV_CUH
#define COPY_HOST_DEV_CUH

#include <vector>
#include "unified_mem.cuh"
#include "dev_mem_texture.cuh"
#include "nd_array_dev.cuh"
#include "../utils/check_error.cuh"
#include "nd_array_texture.cuh"
#include "../../cpp/data/data_constraints.h"

namespace GCT::Data::CUDA
{
template<typename ElemType>
void copyHostToDev(ElemType const *src_h, ElemType *dst_d,
                   size_t num_elems_skip_src, size_t num_elems_skip_dst,
                   size_t num_elems_copy)
{
    GCT::CUDA::checkError(cudaMemcpy(dst_d + num_elems_skip_dst, src_h + num_elems_skip_src,
                                     num_elems_copy * sizeof(ElemType), cudaMemcpyHostToDevice),
                          "cudaMemcpyHostToDevice");
}

// Copy elements stored in STL container (host) to CUDA device array.
template<typename DevArray>
void copyHostToDev(std::span<const typename DevArray::value_type> src, DevArray &dst)
{
    if (src.size() == dst.numElements()) {
        typedef typename DevArray::value_type type;
        auto const num_elems_copy = src.size();
        copyHostToDev<type>(src.data(), dst.dataPointer(), 0u, 0u, num_elems_copy);
    }
    else {
        throw std::runtime_error{
            "Error during host-to-CUDA device copy due to incorrect number of elements.\n"};
    }
}

template<typename DevArray>
void copyHostToDev(std::span<const typename DevArray::value_type> src, DevArray &dst,
                   size_t num_elems_skip_src, size_t num_elems_skip_dst,
                   size_t num_elems_copy)
{
    if ((src.size() >= (num_elems_skip_src + num_elems_copy)) &&
        (dst.numElements() >= (num_elems_skip_dst + num_elems_copy))) {
        typedef typename DevArray::value_type type;
        copyHostToDev<type>(src.data(), dst.dataPointer(),
                            num_elems_skip_src, num_elems_skip_dst,
                            num_elems_copy);
    }
    else {
        throw std::runtime_error{
            "Error during host-to-CUDA device copy due to invalid number of elements.\n"};
    }
}

template<typename ElemType>
void copyDevToHost(ElemType const *src_d, ElemType *dst_h,
                   size_t num_elems_skip_src, size_t num_elems_skip_dst,
                   size_t num_elems_copy)
{
    GCT::CUDA::checkError(cudaMemcpy(dst_h + num_elems_skip_dst, src_d + num_elems_skip_src,
                                     num_elems_copy * sizeof(ElemType), cudaMemcpyDeviceToHost),
                          "cudaMemcpyDeviceToHost");
}

template<typename DevArray>
void copyDevToHost(DevArray const &src, std::span<typename DevArray::value_type> dst)
{
    if (src.numElements() == dst.size()) {
        typedef typename DevArray::value_type type;
        auto const num_elems_copy = dst.size();
        copyDevToHost<type>(src.dataPointer(), dst.data(), 0u, 0u, num_elems_copy);
    }
    else {
        throw std::runtime_error{
            "Error during CUDA device-to-host copy due to incorrect number of elements.\n"};
    }
}

template<typename DevArray>
void copyDevToHost(DevArray const &src, std::span<typename DevArray::value_type> dst,
                   size_t num_elems_skip_src, size_t num_elems_skip_dst,
                   size_t num_elems_copy)
{
    if ((src.numElements() >= (num_elems_skip_src + num_elems_copy)) &&
        (dst.size() >= (num_elems_skip_dst + num_elems_copy))) {
        typedef typename DevArray::value_type type;
        copyDevToHost<type>(src.dataPointer(), dst.data(),
                            num_elems_skip_src, num_elems_skip_dst,
                            num_elems_copy);
    }
    else {
        throw std::runtime_error{
            "Error during CUDA device-to-host copy due to incorrect number of elements.\n"};
    }
}

// Copy from device global memory to specialized texture memory.
template<typename SrcDevArray>
void copyDevToDevTexture(SrcDevArray const &src,
                         Data::CUDA::NDArrayTexture<typename SrcDevArray::value_type, 1u> &dst,
                         unsigned skip_elem = 0)
{
    if (src.numElements() >= (dst.numElements() + skip_elem)) {
        auto const size_bytes = sizeof(typename SrcDevArray::value_type) * dst.numElements();
        GCT::CUDA::checkError(cudaMemcpy2DToArray(dst.dataPointer(),
                                                  0,
                                                  0,
                                                  src.dataPointer() + skip_elem,
                                                  size_bytes,
                                                  size_bytes,
                                                  1u,
                                                  cudaMemcpyDeviceToDevice),
                              "cudaMemcpy2DToArray (cudaMemcpyDeviceToDevice)");
    }
    else {
        throw std::runtime_error{
            "Error during CUDA device(global memory)-to-device(texture) copy due to incorrect number of elements.\n"};
    }
}

// Copy from device global memory to specialized texture memory.
template<typename SrcDevArray>
void copyDevToDevTexture(SrcDevArray const &src,
                         Data::CUDA::NDArrayTexture<typename SrcDevArray::value_type, 2u> &dst,
                         unsigned skip_elem = 0)
{
    if (src.numElements() >= (dst.numElements() + skip_elem)) {
        auto const num_cols = dst.dimensions().front();
        auto const num_rows = dst.dimensions().back();
        typedef typename SrcDevArray::value_type type;
        GCT::CUDA::checkError(cudaMemcpy2DToArray(dst.dataPointer(),
                                                  0,
                                                  0,
                                                  src.dataPointer() + skip_elem,
                                                  num_cols * sizeof(type),
                                                  num_cols * sizeof(type),
                                                  num_rows,
                                                  cudaMemcpyDeviceToDevice),
                              "cudaMemcpy2DToArray (cudaMemcpyDeviceToDevice)");
    }
    else {
        throw std::runtime_error{
            "Error during CUDA device(global memory)-to-device(texture) copy due to incorrect number of elements.\n"};
    }
}

template<typename T>
void copyHostToDevTexture(std::span<const T> src,
                          Data::CUDA::NDArrayTexture<T, 2u> &dst,
                          unsigned skip_elem = 0)
{
    if (src.size() >= (dst.numElements() + skip_elem)) {
        auto const num_cols = dst.dimensions().front();
        auto const num_rows = dst.dimensions().back();
        GCT::CUDA::checkError(cudaMemcpy2DToArray(dst.dataPointer(),
                                                  0,
                                                  0,
                                                  src.data() + skip_elem,
                                                  num_cols * sizeof(T),
                                                  num_cols * sizeof(T),
                                                  num_rows,
                                                  cudaMemcpyHostToDevice),
                              "cudaMemcpy2DToArray (cudaMemcpyHostToDevice)");
    }
    else {
        throw std::runtime_error{
            "Error during CUDA host-to-device(texture) copy due to incorrect number of elements.\n"};
    }
}

}//GCT::Data::CUDA namespace.
#endif
