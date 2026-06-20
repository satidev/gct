#include "timer.cuh"
#include "check_error.cuh"

namespace GCT::CUDA
{
Timer::Timer()
{
    checkError(cudaEventCreate(&start_), "creation of start event for timer");
    checkError(cudaEventCreate(&stop_), "creation of stop event for timer");
}

Timer::~Timer()
{
    auto status = cudaEventDestroy(start_);
    status = cudaEventDestroy(stop_);
}
void Timer::tic()
{
    checkError(cudaEventRecord(start_), "cudaEventRecord");
}

float Timer::toc()
{
    checkError(cudaEventRecord(stop_), "cudaEventRecord for timer stop function");
    checkError(cudaEventSynchronize(stop_), "cudaEventSynchronize for timer stop function");

    auto elapsed_time_ms = 0.0f;
    checkError(cudaEventElapsedTime(&elapsed_time_ms, start_, stop_),
               "timer's elapsed time computation");
    return elapsed_time_ms/1000.0f;
}

}//GCT::CUDA namespace.

