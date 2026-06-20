#ifndef ND_ARRAY_BASE_H
#define ND_ARRAY_BASE_H

#include "data_constraints.h"
#include <array>

namespace GCT::Data
{
// Abstract base class of all arrays.
template<typename ElemType, size_t Dim> requires ValidDim<Dim>
class NDArrayBase
{
protected:
    explicit NDArrayBase(std::array<unsigned, Dim> const &arr_dims);

public:
    using value_type = ElemType;

    std::array<unsigned, Dim> const &dimensions() const noexcept
    {
        return arr_dims_;
    }
    unsigned numElements() const noexcept
    {
        return num_elem_;
    }

private:
    std::array<unsigned, Dim> arr_dims_;
    unsigned num_elem_;
};

template<typename ElemType, size_t Dim>
requires ValidDim<Dim>
NDArrayBase<ElemType, Dim>::NDArrayBase(std::array<unsigned int, Dim> const &arr_dims)
{
    num_elem_ = std::accumulate(std::cbegin(arr_dims), std::cend(arr_dims), 1u, std::multiplies<>());
    if (num_elem_ == 0u) {
        throw std::invalid_argument{
            "Number of elements of NDArray in any dimension cannot be zero.\n"};
    }
    else {
        arr_dims_ = arr_dims;
    }
}
}// GCT::Data namespaces.

#endif //ND_ARRAY_BASE_H
