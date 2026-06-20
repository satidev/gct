#ifndef GCT_GCTL_CPP_IMG_IMG_ND_H_
#define GCT_GCTL_CPP_IMG_IMG_ND_H_

#include <vector>
#include <numeric>
#include <iostream>
#include <functional>
#include "../numeric/comparator.h"
#include "data_constraints.h"
#include "nd_array_base.h"

namespace GCT::Data
{
// N-Dimensional numeric array.
// Base class of projections and reconstructed images.
template<NumericType ElemType, size_t Dim> requires ValidDim<Dim>
class NDArray: public NDArrayBase<ElemType, Dim>
{
public:
    explicit NDArray(std::array<unsigned, Dim> const &arr_dims,
                     ElemType init_val = ElemType{});

    std::vector<ElemType> const &elements() const
    {
        return elems_;
    }
    std::vector<ElemType> &elements()
    {
        return elems_;
    }

    ElemType *dataPointer() noexcept
    {
        return elems_.data();
    }
    ElemType const *dataPointer() const noexcept
    {
        return elems_.data();
    }

    // Scalar arithmetic.
    NDArray &operator+=(ElemType val);
    NDArray &operator-=(ElemType val);
    NDArray &operator*=(ElemType val);
    NDArray &operator/=(ElemType val);

    // Element-wise arithmetic operations.
    NDArray &operator+=(NDArray<ElemType, Dim> const &arr);
    NDArray &operator-=(NDArray<ElemType, Dim> const &arr);
    NDArray &operator*=(NDArray<ElemType, Dim> const &arr);
    NDArray &operator/=(NDArray<ElemType, Dim> const &arr);

    void setVal(ElemType val);
    void setData(std::vector<ElemType> const &data);
    void setData(std::vector<ElemType> &&data);

    [[nodiscard]] bool isEqual(NDArray<ElemType, Dim> const &arr, ElemType eps = ElemType{});

private:

    template<typename ArithFunc>
    void elemWiseArith(NDArray<ElemType, Dim> const &arr);

    template<typename ArithFunc>
    void scalarArith(ElemType val);

protected:
    std::vector<ElemType> elems_;
};

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim>::NDArray(std::array<unsigned, Dim> const &arr_dims, ElemType init_val):
    NDArrayBase<ElemType, Dim>{arr_dims}
{
    try {
        elems_.assign(this->numElements(), init_val);
    }
    catch (std::exception const &e) {
        throw std::runtime_error{"Error during the allocation of NDArray elements.\n"};
    }
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
bool NDArray<ElemType, Dim>::isEqual(NDArray<ElemType, Dim> const &arr, ElemType eps)
{
    return Numeric::Comparator::areEqualArrays<ElemType>(elems_, arr.elements(), eps);
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator+=(ElemType val)
{
    scalarArith<std::plus<ElemType>>(val);
    return *this;
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator-=(ElemType val)
{
    scalarArith<std::minus<ElemType>>(val);
    return *this;
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator*=(ElemType val)
{
    scalarArith<std::multiplies<ElemType>>(val);
    return *this;
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
void NDArray<ElemType, Dim>::setVal(ElemType val)
{
    std::fill(std::begin(elems_), std::end(elems_), val);
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator/=(ElemType val)
{
    scalarArith<std::divides<ElemType>>(val);
    return *this;
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
template<typename ArithFunc>
void NDArray<ElemType, Dim>::scalarArith(ElemType val)
{
    auto const func = ArithFunc{};
    std::for_each(std::begin(elems_), std::end(elems_),
                  [func, val](ElemType &elem)
                  {
                      elem = func(elem, val);
                  }
    );
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
template<typename ArithFunc>
void NDArray<ElemType, Dim>::elemWiseArith(NDArray<ElemType, Dim> const &arr)
{
    auto const &vec = arr.elements();
    if (arr.dimensions() == this->dimensions()) {
        std::transform(std::cbegin(elems_), std::cend(elems_), std::cbegin(vec),
                       std::begin(elems_), ArithFunc{});
    }
    else {
        throw std::invalid_argument{
            "Error during the element-wise arithmetic operation due to dimension mismatch.\n"};
    }
}
template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator+=(NDArray<ElemType, Dim> const &arr)
{
    elemWiseArith<std::plus<ElemType>>(arr);
    return *this;
}
template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator-=(NDArray<ElemType, Dim> const &arr)
{
    elemWiseArith<std::minus<ElemType>>(arr);
    return *this;
}
template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator*=(NDArray<ElemType, Dim> const &arr)
{
    elemWiseArith<std::multiplies<ElemType>>(arr);
    return *this;
}
template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
NDArray<ElemType, Dim> &NDArray<ElemType, Dim>::operator/=(NDArray<ElemType, Dim> const &arr)
{
    elemWiseArith<std::divides<ElemType>>(arr);
    return *this;
}

template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
void NDArray<ElemType, Dim>::setData(std::vector<ElemType> const &data)
{
    if (this->numElements() == data.size()) {
        std::copy(std::cbegin(data), std::cend(data), std::begin(elems_));
    }
    else {
        throw std::invalid_argument{
            "Cannot copy data from the STL vector to NDArray due to incorrect number of elements.\n"};
    }
}
template<NumericType ElemType, size_t Dim>
requires ValidDim<Dim>
void NDArray<ElemType, Dim>::setData(std::vector<ElemType> &&data)
{
    if (this->numElements() == data.size()) {
        elems_ = std::move(data);
    }
    else {
        throw std::invalid_argument{
            "Cannot move data from the STL vector to NDArray due to incorrect number of elements.\n"};
    }
}
}//GCT::Data namespace.

#endif //GCT_GCTL_CPP_IMG_IMG_ND_H_
