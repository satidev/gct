#ifndef DEV_MEM_CUH
#define DEV_MEM_CUH

#include "read_write_mem_resource.cuh"
#include "../utils/check_error.cuh"

namespace GCT::Data::CUDA
{
// For the allocation of data in CUDA device memory using cudaMalloc.
struct cudaMallocFunctor
{
    void operator()(void **ptr, size_t bytes)
    {
        GCT::CUDA::checkError(cudaMalloc(ptr, bytes), "cudaMalloc");
    }
};

template<typename T>
class DevMem: public ReadWriteMemResource<T>
{
public:
    explicit DevMem(unsigned num_elem);
    DevMem(DevMem &&data) noexcept;

    ~DevMem()
    {
        cudaFree(this->buff_ptr_);
    }
};

template<typename T>
DevMem<T>::DevMem(DevMem &&data) noexcept
{
    this->num_elem_ = data.num_elem_;
    this->buff_ptr_ = data.buff_ptr_;

    data.num_elem_ = 0u;
    data.buff_ptr_ = nullptr;
}
template<typename T>
DevMem<T>::DevMem(unsigned num_elem)
try
    :ReadWriteMemResource<T>{num_elem, allocate<T, cudaMallocFunctor>(sizeof(T) * num_elem)}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Error during the allocation of memory resource in CUDA device.\n" +
            std::string{e.what()}
    };
}
catch (std::runtime_error const &e) {
    throw std::runtime_error{
        "Error during the allocation of memory resource in CUDA device.\n" +
            std::string{e.what()}};
}
}//GCT::Data::CUDA namespace.
#endif //DEV_MEM_CUH
