#ifndef VEC2D_MATH_CUH
#define VEC2D_MATH_CUH

#include "../../cpp/geom/vec_wcs.h"

namespace GCT::Numeric::Vec2DMath::CUDA
{
__device__ __host__ inline float2 operator+(float2 const &a, float2 const &b);
__device__ __host__ inline float2 operator-(float2 const &a, float2 const &b);
__device__ __host__ inline float2 operator*(float2 const &a, float2 const &b);
__device__ __host__ inline float2 operator/(float2 const &a, float2 const &b);
__device__ __host__ inline float2 operator/(float2 const &a, float b);
__device__ __host__ inline float dot(float2 const &a, float2 const &b);
__device__ __host__ inline float norm(float2 const &a);
__device__ __host__ inline float2 operator*(float2 const &a, float b);
__device__ __host__ inline float2 operator+(float2 const &a, float s);

__device__ __host__ inline float2 operator+(float2 const &a, float2 const &b)
{
    return make_float2(a.x + b.x, a.y + b.y);
}

__device__ __host__ inline float2 operator-(float2 const &a, float2 const &b)
{
    return make_float2(a.x - b.x, a.y - b.y);
}

__device__ __host__ inline float2 operator*(float2 const &a, float2 const &b)
{
    return make_float2(a.x * b.x, a.y * b.y);
}

__device__ __host__ inline float2 operator/(float2 const &a, float2 const &b)
{
    return make_float2(a.x / b.x, a.y / b.y);
}

__device__ __host__ inline float2 operator/(float2 const &a, float b)
{
    return make_float2(a.x / b, a.y / b);
}

__device__ __host__ inline float dot(float2 const &a, float2 const &b)
{
    auto const prod = a * b;
    return prod.x + prod.y;
}

__device__ __host__ inline float norm(float2 const &a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

__device__ __host__ inline float2 operator*(float2 const &a, float s)
{
    return make_float2(a.x * s, a.y * s);
}
__device__ __host__ inline float2 operator+(float2 const &a, float s)
{
    return make_float2(a.x + s, a.y + s);
}

}//GCT::Numeric::Vec2DMath::CUDA namespace.
#endif //VEC2D_MATH_CUH
