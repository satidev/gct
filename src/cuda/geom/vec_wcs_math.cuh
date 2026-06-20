#ifndef VEC_WCS_MATH_CUH
#define VEC_WCS_MATH_CUH

#include "../../cpp/geom/vec_wcs.h"
#include "../numeric/arithm_functors.cuh"

namespace GCT::Geom::CUDA
{
template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator+(VecWCS<Dim> const &a, VecWCS<Dim> const &b);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator-(VecWCS<Dim> const &a, VecWCS<Dim> const &b);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator*(VecWCS<Dim> const &a, VecWCS<Dim> const &b);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator/(VecWCS<Dim> const &a, VecWCS<Dim> const &b);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator+(VecWCS<Dim> const &vec, float scalar);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator-(VecWCS<Dim> const &vec, float scalar);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator*(VecWCS<Dim> const &vec, float scalar);

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator/(VecWCS<Dim> const &vec, float scalar);

__device__ __host__ inline float norm(VecWCS<2u> const &vec);
__device__ __host__ inline float norm(VecWCS<3u> const &vec);

__device__ __host__ inline float sum(VecWCS<2u> const &vec);
__device__ __host__ inline float sum(VecWCS<3u> const &vec);

template<size_t Dim>
__device__ __host__ inline float dot(VecWCS<Dim> const &a, VecWCS<Dim> const &b);

namespace Detail
{
template<typename ArithFunc>
__device__ __host__ inline void elementWiseOp(VecWCS<2u> const &a,
                                              VecWCS<2u> const &b,
                                              VecWCS<2u> &res,
                                              ArithFunc func);
template<typename ArithFunc>
__device__ __host__ inline void elementWiseOp(VecWCS<3u> const &a,
                                              VecWCS<3u> const &b,
                                              VecWCS<3u> &res,
                                              ArithFunc func);

template<typename ArithFunc>
__device__ __host__ inline void scalarOp(VecWCS<2u> const &vec,
                                         float scalar,
                                         VecWCS<2u> &res,
                                         ArithFunc func);
template<typename ArithFunc>
__device__ __host__ inline void scalarOp(VecWCS<3u> const &vec,
                                         float scalar,
                                         VecWCS<3u> &res,
                                         ArithFunc func);

template<typename ArithFunc>
__device__ __host__ inline void elementWiseOp(VecWCS<2u> const &a,
                                              VecWCS<2u> const &b,
                                              VecWCS<2u> &res,
                                              ArithFunc func)
{
    res.x.val = func(a.x.val, b.x.val);
    res.y.val = func(a.y.val, b.y.val);
}

template<typename ArithFunc>
__device__ __host__ inline void elementWiseOp(VecWCS<3u> const &a,
                                              VecWCS<3u> const &b,
                                              VecWCS<3u> &res,
                                              ArithFunc func)
{
    res.x.val = func(a.x.val, b.x.val);
    res.y.val = func(a.y.val, b.y.val);
    res.z.val = func(a.z.val, b.z.val);
}

template<typename ArithFunc>
__device__ __host__ inline void scalarOp(VecWCS<2u> const &vec,
                                         float scalar,
                                         VecWCS<2u> &res,
                                         ArithFunc func)
{
    res.x.val = func(vec.x.val, scalar);
    res.y.val = func(vec.y.val, scalar);
}

template<typename ArithFunc>
__device__ __host__ inline void scalarOp(VecWCS<3u> const &vec,
                                         float scalar,
                                         VecWCS<3u> &res,
                                         ArithFunc func)
{
    res.x.val = func(vec.x.val, scalar);
    res.y.val = func(vec.y.val, scalar);
    res.z.val = func(vec.z.val, scalar);
}

}//Detail namespace

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator+(VecWCS<Dim> const &a, VecWCS<Dim> const &b)
{
    auto res = a;
    Detail::elementWiseOp(a, b, res, Numeric::CUDA::PlusOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator-(VecWCS<Dim> const &a, VecWCS<Dim> const &b)
{
    auto res = a;
    Detail::elementWiseOp(a, b, res, Numeric::CUDA::MinusOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator*(VecWCS<Dim> const &a, VecWCS<Dim> const &b)
{
    auto res = a;
    Detail::elementWiseOp(a, b, res, Numeric::CUDA::MulOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator/(VecWCS<Dim> const &a, VecWCS<Dim> const &b)
{
    auto res = a;
    Detail::elementWiseOp(a, b, res, Numeric::CUDA::DivideOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator+(VecWCS<Dim> const &vec, float scalar)
{
    auto res = vec;
    Detail::scalarOp(vec, scalar, res, Numeric::CUDA::PlusOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator-(VecWCS<Dim> const &vec, float scalar)
{
    auto res = vec;
    Detail::scalarOp(vec, scalar, res, Numeric::CUDA::MinusOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator*(VecWCS<Dim> const &vec, float scalar)
{
    auto res = vec;
    Detail::scalarOp(vec, scalar, res, Numeric::CUDA::MulOp{});
    return res;
}

template<size_t Dim>
__device__ __host__ inline VecWCS<Dim> operator/(VecWCS<Dim> const &vec, float scalar)
{
    auto res = vec;
    Detail::scalarOp(vec, scalar, res, Numeric::CUDA::DivideOp{});
    return res;
}

__device__ __host__ inline float norm(VecWCS<2u> const &vec)
{
    return sqrtf(vec.x.val * vec.x.val + vec.y.val * vec.y.val);
}

__device__ __host__ inline float norm(VecWCS<3u> const &vec)
{
    return sqrtf(vec.x.val * vec.x.val + vec.y.val * vec.y.val + vec.z.val * vec.z.val);
}

__device__ __host__ inline float sum(VecWCS<2u> const &vec)
{
    return vec.x.val + vec.y.val;
}

__device__ __host__ inline float sum(VecWCS<3u> const &vec)
{
    return vec.x.val + vec.y.val + vec.z.val;
}

template<size_t Dim>
__device__ __host__ inline float dot(VecWCS<Dim> const &a, VecWCS<Dim> const &b)
{
    auto const mul = a * b;
    return sum(mul);
}

}//GCT::Geom::CUDA namespace.

#endif //VEC_WCS_MATH_CUH
