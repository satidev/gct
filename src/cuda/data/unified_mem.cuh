#ifndef UNIFIED_MEM_CUH
#define UNIFIED_MEM_CUH

#include "read_write_mem_resource.cuh"
#include "../utils/check_error.cuh"

namespace GCT::Data::CUDA
{
// For the allocation of data in CUDA unified memory.
struct cudaAllocUnifiedFunctor
{
    void operator()(void **ptr, size_t bytes)
    {
        GCT::CUDA::checkError(cudaMallocManaged(ptr, bytes), "cudaMallocManaged");
    }
};

// For the allocation of data in CUDA unified memory.
template<typename T>
class UnifiedMem: public ReadWriteMemResource<T>
{
public:
    explicit UnifiedMem(unsigned num_elem);
    UnifiedMem(UnifiedMem &&data) noexcept;

    ~UnifiedMem()
    {
        cudaFree(this->buff_ptr_);
    }
};

template<typename T>
UnifiedMem<T>::UnifiedMem(UnifiedMem &&data) noexcept
{
    this->num_elem_ = data.num_elem_;
    this->buff_ptr_ = data.buff_ptr_;

    data.num_elem_ = 0u;
    data.buff_ptr_ = nullptr;
}
template<typename T>
UnifiedMem<T>::UnifiedMem(unsigned num_elem)
try
    :
    ReadWriteMemResource<T>{num_elem, allocate<T, cudaAllocUnifiedFunctor>(sizeof(T) * num_elem)}
{

}
catch (std::invalid_argument const &e) {
    throw std::invalid_argument{
        "Error during the allocation of CUDA unified memory resource.\n" +
            std::string{e.what()}
    };
}
catch (std::runtime_error const &e) {
    throw std::runtime_error{
        "Error during the allocation of CUDA unified memory resource.\n" +
            std::string{e.what()}};
}
}//GCT::Data::CUDA namespace.


#endif //UNIFIED_MEM_CUH
