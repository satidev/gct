#ifndef TIMER_CUH
#define TIMER_CUH

namespace GCT::CUDA
{
//Timer for cuda events and kernels.
class Timer
{
public:
    //TODO: Add stream and device info.
    Timer();
    ~Timer();

    //Start the clock.
    void tic();
    //End the clock and return elapsed time in seconds.
    float toc();

private:
    cudaEvent_t start_;
    cudaEvent_t stop_;
};

}//GCT::CUDA namespace.


#endif //TIMER_CUH
