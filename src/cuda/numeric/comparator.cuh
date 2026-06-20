#ifndef COMPARATOR_CUH
#define COMPARATOR_CUH

#include <thrust/device_vector.h>
#include <thrust/logical.h>
#include <thrust/execution_policy.h>

namespace GCT::Geom::Comparator::CUDA
{
template<typename T>
__device__ __host__ constexpr bool isPositiveInf(T val)
{
    return (val > 0) && (isinf(val));
}

template<typename T>
__device__ __host__ constexpr bool isNegativeInf(T val)
{
    return (val < 0) && isinf(val);
}

template<typename T>
__device__ __host__ constexpr bool isEqual(T val_1, T val_2, T eps)
{
    if ((isnan(val_1) && isnan(val_2)) ||
        (isPositiveInf(val_1) && isPositiveInf(val_2)) ||
        (isNegativeInf(val_1) && isNegativeInf(val_2))) {
        return true;
    }
    else {
        return fabs(val_1 - val_2) <= eps;
    }
}

//Functor to check a thrust container has the same value for all it's elements.
template<typename T>
struct HasSameValFunctor
{
    __device__ __host__ explicit HasSameValFunctor(T val, T t_eps)
        : val{val}, eps{t_eps}
    {}

    __device__ __host__ bool operator()(T elem) const
    {
        return isEqual(val, elem, eps);
    }
private:
    T val;
    T eps;
};

template<typename T>
struct IsEqualVecFunctor
{
    __device__ __host__ explicit IsEqualVecFunctor(T t_eps)
        : eps{t_eps}
    {}
    __device__ __host__ bool operator()(T elem_1, T elem_2) const
    {
        return isEqual(elem_1, elem_2, eps);
    }
private:
    T eps;
};

template<typename T>
bool hasSameVal(thrust::device_vector<T> const &vec,
                T val, T eps)
{
    return thrust::all_of(thrust::device, vec.cbegin(),
                          vec.cend(), HasSameValFunctor<T>(val, eps));
}

template<typename Container>
bool isEqualVec(Container const &vec_1, Container const &vec_2,
                typename Container::value_type eps)
{
    if (vec_1.size() == vec_2.size()) {
        return thrust::equal(vec_1.cbegin(), vec_1.cend(), vec_2.cbegin(),
                             IsEqualVecFunctor<typename Container::value_type>(eps));
    }
    else {
        return false;
    }
}

}//GCT::Numeric::Comparator::CUDA namespace.

#endif //COMPARATOR_CUH
