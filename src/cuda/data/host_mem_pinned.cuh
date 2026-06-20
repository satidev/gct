#ifndef HOST_MEM_PINNED_CUH
#define HOST_MEM_PINNED_CUH

#include "read_write_mem_resource.cuh"
#include "../utils/check_error.cuh"

namespace GCT::Data::CUDA
{
//For the allocation of data in host-pinned memory.
struct cudaHostAllocFunctor
{
    void operator()(void **ptr, size_t bytes)
    {
        GCT::CUDA::checkError(cudaMallocHost(ptr, bytes), "cudaMallocHost");
    }
};

template<typename T>
class HostMemPinned: public ReadWriteMemResource<T>
{
public:
    explicit HostMemPinned(unsigned num_elem);
    HostMemPinned(HostMemPinned &&data) noexcept;

    ~HostMemPinned()
    {
        cudaFreeHost(this->buff_ptr_);
    }
};
template<typename T>
HostMemPinned<T>::HostMemPinned(HostMemPinned &&data) noexcept
{
    this->num_elem_ = data.num_elem_;
    this->buff_ptr_ = data.buff_ptr_;

    data.num_elem_ = 0u;
    data.buff_ptr_ = nullptr;
}
template<typename T>
HostMemPinned<T>::HostMemPinned(unsigned num_elem)
try
    :ReadWriteMemResource<T>{num_elem, allocate<T, cudaHostAllocFunctor>(sizeof(T) * num_elem)}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Error during the allocation of pinned memory resource in host.\n" +
            std::string{e.what()}
    };
}
catch (std::runtime_error const &e) {
    throw std::runtime_error{
        "Error during the allocation of pinned memory resource in host.\n" +
        std::string{e.what()}};
}
}//GCT::Data::CUDA namespace.

#endif //HOST_MEM_PINNED_CUH
