#ifndef VEC3D_MATH_CUH
#define VEC3D_MATH_CUH

#include "../../cpp/geom/vec_wcs.h"

namespace GCT::Numeric::Vec3DMath::CUDA
{
__device__ __host__ inline float3 operator+(float3 const &a, float3 const &b);
__device__ __host__ inline float3 operator-(float3 const &a, float3 const &b);
__device__ __host__ inline float3 operator*(float3 const &a, float3 const &b);
__device__ __host__ inline float3 operator/(float3 const &a, float3 const &b);
__device__ __host__ inline float3 operator/(float3 const &a, float b);
__device__ __host__ inline float dot(float3 const &a, float3 const &b);
__device__ __host__ inline float dot(float4 const &a, float4 const &b);
__device__ __host__ inline float norm(float3 const &a);

__device__ __host__ inline float3 operator+(float3 const &a, float3 const &b)
{
    return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

__device__ __host__ inline float3 operator-(float3 const &a, float3 const &b)
{
    return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

__device__ __host__ inline float3 operator*(float3 const &a, float3 const &b)
{
    return make_float3(a.x * b.x, a.y * b.y, a.z * b.z);
}

__device__ __host__ inline float3 operator/(float3 const &a, float3 const &b)
{
    return make_float3(a.x / b.x, a.y / b.y, a.z / b.z);
}

__device__ __host__ inline float3 operator/(float3 const &a, float b)
{
    return make_float3(a.x / b, a.y / b, a.z / b);
}

__device__ __host__ inline float dot(float3 const &a, float3 const &b)
{
    float3 const prod = a * b;
    return prod.x + prod.y + prod.z;
}

__device__ __host__ inline float dot(float4 const &a, float4 const &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

__device__ __host__ inline float norm(float3 const &a)
{
    return sqrtf(a.x*a.x + a.y*a.y + a.z*a.z);
}

}//GCT::Numeric::Vec3DMath::CUDA namespace.
#endif //VEC3D_MATH_CUH
