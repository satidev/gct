#ifndef DEV_MEM_TEXTURE_CUH
#define DEV_MEM_TEXTURE_CUH

#include <array>
#include <numeric>
#include "../utils/check_error.cuh"

namespace GCT::Data::CUDA
{
//For the allocation data in cudaArray (read-only memory optimized for binding to textures).
template<typename ElemType>
class DevMemTexture
{
public:
    explicit DevMemTexture(std::array<unsigned, 1u> const &dims);
    explicit DevMemTexture(std::array<unsigned, 2u> const &dims);
    explicit DevMemTexture(std::array<unsigned, 3u> const &dims);

    cudaArray const *dataPointer() const noexcept
    {
        return arr_;
    }
    cudaArray *dataPointer() noexcept
    {
        return arr_;
    }
    unsigned numElements() const noexcept
    {
        return num_elem_;
    }

    ~DevMemTexture()
    {
        cudaFreeArray(arr_);
    }

    //Since GPU resources are scarce, only move constructor is allowed.
    DevMemTexture(DevMemTexture const &) = delete;
    DevMemTexture(DevMemTexture &&data) noexcept;
    DevMemTexture &operator=(DevMemTexture const &) = delete;
    DevMemTexture &operator=(DevMemTexture &&) = delete;

private:
    unsigned num_elem_;
    cudaArray *arr_ = nullptr;
};

template<typename ElemType>
DevMemTexture<ElemType>::DevMemTexture(std::array<unsigned, 3u> const &dims)
{
    num_elem_ = dims[0u] * dims[1u] * dims[2u];
    auto extent = cudaExtent{};
    extent.width = dims[0u];
    extent.height = dims[1u];
    extent.depth = dims[2u];

    auto const channel_desc = cudaCreateChannelDesc<ElemType>();
    checkError(cudaMalloc3DArray(static_cast<cudaArray **>(&arr_), &channel_desc, extent),
               "cudaMalloc3DArray (memory for texture binding)");
}

template<typename ElemType>
DevMemTexture<ElemType>::DevMemTexture(DevMemTexture &&data) noexcept
{
    num_elem_ = data.num_elem_;
    arr_ = data.arr_;
    data.arr_ = nullptr;
}

template<typename ElemType>
DevMemTexture<ElemType>::DevMemTexture(std::array<unsigned, 1u> const &dims)
{
    num_elem_ = dims[0u];
    auto const channel_desc = cudaCreateChannelDesc<ElemType>();
    GCT::CUDA::checkError(cudaMallocArray(static_cast<cudaArray **>(&arr_), &channel_desc, num_elem_, 1u),
               "cudaMallocArray (memory for texture binding)");
}

template<typename ElemType>
DevMemTexture<ElemType>::DevMemTexture(std::array<unsigned, 2u> const &dims)
{
    num_elem_ = dims[0u] * dims[1u];
    auto const channel_desc = cudaCreateChannelDesc<ElemType>();
    GCT::CUDA::checkError(cudaMallocArray(static_cast<cudaArray **>(&arr_), &channel_desc, dims[0u], dims[1u]),
               "cudaMallocArray (memory for texture binding)");
}

}//GCT::Data::CUDA namespace.
#endif //DEV_MEM_TEXTURE_CUH
