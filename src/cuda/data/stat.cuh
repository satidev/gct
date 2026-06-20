#ifndef STAT_CUH
#define STAT_CUH

#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/execution_policy.h>

namespace GCT::Data::Stat::CUDA
{
template<typename DevArray>
double sum(DevArray const &arr)
{
    thrust::device_ptr<typename DevArray::value_type const> const dev_ptr(arr.dataPointer());
    return thrust::reduce(thrust::device, dev_ptr, dev_ptr + arr.numElements(), double{0.0});
}

template<typename DevArray>
double mean(DevArray const &arr)
{
    return sum(arr)/static_cast<double>(arr.numElements());
}

}//GCT::Data::Stat::CUDA namespace.

#endif //STAT_CUH
