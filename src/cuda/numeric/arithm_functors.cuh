#ifndef ARITHM_FUNCTORS_CUH
#define ARITHM_FUNCTORS_CUH

namespace GCT::Numeric::CUDA
{
struct PlusOp
{
    __device__ __host__ float operator()(float a, float b)
    {
        return a + b;
    }
};
struct MinusOp
{
    __device__ __host__ float operator()(float a, float b)
    {
        return a - b;
    }
};

struct MulOp
{
    __device__ __host__ float operator()(float a, float b)
    {
        return a * b;
    }
};
struct DivideOp
{
    __device__ __host__ float operator()(float a, float b)
    {
        return a / b;
    }
};
}//GCT::Numeric::CUDA namespace.

#endif //ARITHM_FUNCTORS_CUH
