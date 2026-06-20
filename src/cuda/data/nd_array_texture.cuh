#ifndef ND_ARRAY_TEXTURE_CUH
#define ND_ARRAY_TEXTURE_CUH

#include <array>
#include "../utils/cuda_config.cuh"
#include "dev_mem_texture.cuh"
#include "../../cpp/data/nd_array_base.h"

namespace GCT::Data::CUDA
{
// 1-,2- and 3-D arrays where the elements are in read-only texture memory.
template<typename ElemType, size_t Dim> requires (Dim > 0u) || (Dim<size_t{4u})
class NDArrayTexture: public NDArrayBase<ElemType, Dim>
{
public:
    explicit NDArrayTexture(std::array<unsigned, Dim> const &size);

    cudaArray *dataPointer() noexcept
    {
        return data_.dataPointer();
    }
    cudaTextureObject_t texObject() const noexcept
    {
        return tex_;
    }

private:
    DevMemTexture<ElemType> data_;
    std::array<unsigned, Dim> dims_;
    cudaTextureObject_t tex_ = 0;
};

template<typename PixType, size_t Dim> requires (Dim > 0u) || (Dim<size_t{4u})
NDArrayTexture<PixType, Dim>::NDArrayTexture(std::array<unsigned, Dim> const &size)
try
    : NDArrayBase<PixType, Dim>{size},
      data_{size}, dims_{size}
{
    auto res_desc = cudaResourceDesc{};
    res_desc.resType = cudaResourceTypeArray;
    res_desc.res.array.array = data_.dataPointer();

    auto tex_desc = cudaTextureDesc{};
    tex_desc.addressMode[0] = cudaAddressModeBorder;
    if constexpr (Dim == 2u) {
        tex_desc.addressMode[1] = cudaAddressModeBorder;
    }
    tex_desc.filterMode = cudaFilterModeLinear;
    tex_desc.readMode = cudaReadModeElementType;
    tex_desc.normalizedCoords = false;

    auto const status = cudaCreateTextureObject(&tex_, &res_desc, &tex_desc, nullptr);
    if (status != cudaSuccess) {
        throw std::runtime_error{"Error while creating texture object.\n"};
    }
}
catch (std::exception const &e) {
    throw std::runtime_error{
        "Error during the allocation of buffer for texture binding.\n" +
            std::string{e.what()}};
}
}//GCT::Data::CUDA namespace.

#endif //ND_ARRAY_TEXTURE_CUH


