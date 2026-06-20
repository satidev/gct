#ifndef READ_WRITE_MEM_RESOURCE_CUH
#define READ_WRITE_MEM_RESOURCE_CUH

#include <stdexcept>

namespace GCT::Data::CUDA
{
template<typename T, typename cudaAlloc>
T *allocate(size_t bytes)
{
    if (bytes == 0) {
        throw std::invalid_argument{"The number of elements cannot be zero.\n"};
    }
    void *dev_ptr;
    cudaAlloc{}(&dev_ptr, bytes);
    return static_cast<T *>(dev_ptr);
}
// Read-write memory resource.
template<typename T>
class ReadWriteMemResource
{
public:
    using value_type = T;

    unsigned numElements() const noexcept
    {
        return num_elem_;
    }
    T const *dataPointer() const noexcept
    {
        return buff_ptr_;
    }
    T *dataPointer() noexcept
    {
        return buff_ptr_;
    }

    virtual ~ReadWriteMemResource() = 0;

    //Since GPU resources are scarce, currently only move construction is allowed.
    //TODO: Move constructor can also be deleted in C++20 (guaranteed copy elision).
    ReadWriteMemResource(ReadWriteMemResource const &) = delete;
    ReadWriteMemResource &operator=(ReadWriteMemResource const &) = delete;
    ReadWriteMemResource &operator=(ReadWriteMemResource &&) = delete;

protected:
    ReadWriteMemResource() = default;
    ReadWriteMemResource(unsigned num_elem, T *buff_ptr)
        : num_elem_{num_elem}, buff_ptr_{buff_ptr}
    {}

    unsigned num_elem_ = 0u;
    T *buff_ptr_ = nullptr;
};
template<typename T>
ReadWriteMemResource<T>::~ReadWriteMemResource() = default;

}//GCT::Data::CUDA namespace.


#endif //READ_WRITE_MEM_RESOURCE_CUH
