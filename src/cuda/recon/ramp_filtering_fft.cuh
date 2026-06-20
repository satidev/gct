#ifndef RAMP_FILTERING_FFT_CUH
#define RAMP_FILTERING_FFT_CUH

#include "../../cpp/recon/proj_filt_kernel.h"
#include "../../cpp/io/parameters/filt_parameters.h"
#include "../data/vector_dev.cuh"
#include "../data/array_2d_dev.cuh"
#include "../data/copy_host_dev.cuh"
#include <cufft.h>
#include "../utils/cuda_config.cuh"
#include "../data/dev_mem.cuh"

namespace GCT::Recon
{
// Row-wise ramp filtering (using FFT) of projections.
class RampFilteringFFT
{
public:
    template<typename DetRow>
    RampFilteringFFT(DetRow const &dr, FiltParams const &params,
                     unsigned num_rows_filtered);

    // For filtering single or stack of 1D or 2D floating point projections.
    template<typename ProjFloatCUDev>
    void runFiltering(ProjFloatCUDev &proj);

    // Computes the required memory for simultaneous filtering of all rows of
    // projection images.
    // Memory is used for allocating temporary objects for padded projections and
    // FFT computation.
    template<size_t Dim>
    static size_t reqMemory(Data::ProjectionStack<float, Dim> const &proj);

private:
    // Compute the smallest power of 2 greater than a number.
    static uint nextPow2(uint num)
    {
        auto val = 1u;
        while (val < num)
            val *= 2u;
        return val;
    }

    template<typename DetRow>
    static unsigned FFTLength(DetRow const &dr) noexcept
    {
        return nextPow2(2u * dr.sampler.num_samples);
    }

    // Total number of rows (1D signal/data) to be filtered.
    template<size_t Dim>
    static unsigned numRows(std::array<unsigned, Dim> const &size);

    // Generate padded projection.
    template<typename Proj>
    void setProjPadded(Proj const &proj);

    template<size_t Dim>
    static void FFT1D(
        Data::CUDA::NDArrayDev<float, Dim, Data::CUDA::DevMem<float>> const &ip,
        Data::CUDA::NDArrayDev<cufftComplex, Dim, Data::CUDA::DevMem<cufftComplex>> &fft);

    void multFFT();
    void FFT1DInv();

    // Extract relevant region from padded projection.
    template<typename Proj>
    void extractProj(Proj &proj);

    unsigned fft_len_;
    unsigned fft_len_herm_;//FFT len utilizing hermitian symmetry.
    unsigned num_rows_filtered_;
    Data::CUDA::VectorDev<cufftComplex, Data::CUDA::DevMem<cufftComplex>> filt_padded_fft_;
    // Row by row stack of projections irrespective of projection dimensions.
    Data::CUDA::Array2DDev<float, Data::CUDA::DevMem<float>> proj_padded_;
    Data::CUDA::Array2DDev<cufftComplex, Data::CUDA::DevMem<cufftComplex>> proj_padded_fft_;
};

template<typename DetRow>
RampFilteringFFT::RampFilteringFFT(DetRow const &dr, FiltParams const &params,
                                   unsigned num_rows_filtered)
try
    :
    fft_len_{FFTLength(dr)},
    fft_len_herm_{fft_len_ / 2u + 1u},
    num_rows_filtered_{num_rows_filtered},
    filt_padded_fft_{fft_len_herm_},
    proj_padded_{fft_len_, num_rows_filtered},
    proj_padded_fft_{fft_len_herm_, num_rows_filtered}
{
    // Compute filter kernel in spatial domain.
    auto const filt_sp_host = projFiltKernel(dr, params);
    // Zero-padded filter.
    auto filt_sp_padded = std::vector<float>(fft_len_, 0.0);
    std::copy(std::begin(filt_sp_host), std::end(filt_sp_host), std::begin(filt_sp_padded));

    // Copy the filter kernel to CUDA device.
    auto filt_sp_dev = Data::CUDA::VectorDev<float, Data::CUDA::DevMem<float>>{fft_len_};
    try {
        Data::CUDA::copyHostToDev(filt_sp_padded, filt_sp_dev);
    }
    catch (std::exception const &e) {
        throw std::runtime_error{"Error while transferring ramp filter kernel to CUDA device.\n" +
            std::string{e.what()}};
    }
    FFT1D(filt_sp_dev, filt_padded_fft_);
}
catch (std::exception const &e) {
    throw std::runtime_error{
        std::string{"Error during the initialization of projection ramp filtering.\n"} + e.what()};
}

// Extract filtered projections from padded filtered projection.
__global__ void extractProjKern(float const *proj_filt,
                                unsigned num_cols_proj_filt,
                                unsigned num_cols_proj,
                                unsigned num_rows_proj,
                                float *proj);
__device__ inline cufftComplex multComplex(cufftComplex a, cufftComplex b);
__global__ void multFFTKern(cufftComplex const *filt,
                            unsigned num_cols,
                            unsigned num_rows,
                            cufftComplex *op);
__global__ void copyProjToPaddedProjKern(float const *proj,
                                         unsigned num_cols_proj,
                                         unsigned num_cols_proj_padded,
                                         unsigned num_rows,
                                         float *proj_padded);

template<typename Proj>
void RampFilteringFFT::extractProj(Proj &proj)
{
    auto const num_cols_proj = proj.dimensions().at(0u);
    auto const num_filt_rows = num_rows_filtered_;
    auto const config = GCT::CUDA::execConfig(std::array<unsigned, 2u>{num_cols_proj,
                                                                            num_filt_rows});
    extractProjKern<<<config.first, config.second>>>(proj_padded_.dataPointer(),
                                                     proj_padded_.numCols(),
                                                     num_cols_proj, num_filt_rows,
                                                     proj.dataPointer());
    GCT::CUDA::checkErrorKernel("copy projection after FFT filtering");
}

template<size_t Dim>
size_t RampFilteringFFT::reqMemory(Data::ProjectionStack<float, Dim> const &proj)
{
    auto const num_cols = proj.projSize().num_cols;
    auto const tot_num_rows = Data::numProjRows(proj.projSize()) * proj.numProj();
    auto const fft_len = nextPow2(2u * num_cols);
    auto const fft_len_herm = fft_len / 2u + 1u;
    auto const filt_size_bytes = fft_len_herm * sizeof(float2);
    auto const padd_proj_bytes = fft_len * tot_num_rows * sizeof(float);
    auto const padd_proj_fft_bytes = fft_len_herm * tot_num_rows * sizeof(float2);
    return filt_size_bytes + padd_proj_bytes + padd_proj_fft_bytes;
}

template<size_t Dim>
void RampFilteringFFT::FFT1D(
    Data::CUDA::NDArrayDev<float, Dim, Data::CUDA::DevMem<float>> const &ip,
    Data::CUDA::NDArrayDev<cufftComplex, Dim, Data::CUDA::DevMem<cufftComplex>> &fft)
{
    auto plan = cufftHandle{};
    auto const batch_size = static_cast<int>(numRows(ip.dimensions()));
    auto rank = 1;
    auto num_samples = static_cast<int>(ip.dimensions().front());
    auto cufft_status = cufftPlanMany(&plan, rank, &num_samples,
                                      nullptr, 1, 0,
                                      nullptr, 1, 0,
                                      CUFFT_R2C, batch_size);
    if (cufft_status != CUFFT_SUCCESS) {
        throw std::runtime_error{std::string{"Error during FFT computation for ramp filtering.\n"} +
            "Invalid FFT plan.\n"};
    }

    cufft_status = cufftExecR2C(plan, (cufftReal *) (ip.dataPointer()), fft.dataPointer());
    if (cufft_status != CUFFT_SUCCESS) {
        throw std::runtime_error{std::string{"Error during FFT computation for ramp filtering.\n"} +
            "Execution error.\n"};
    }
    cufftDestroy(plan);
}

template<typename Proj>
void RampFilteringFFT::setProjPadded(Proj const &proj)
{
    proj_padded_.setVal(0.0f);
    auto const num_cols_proj = proj.dimensions().at(0);
    auto const num_cols_proj_padded = fft_len_;
    auto const num_rows = num_rows_filtered_;
    auto const config = GCT::CUDA::execConfig(std::array<unsigned, 2u>{num_cols_proj, num_rows});
    copyProjToPaddedProjKern<<<config.first, config.second>>>(proj.dataPointer(), num_cols_proj,
                                                              num_cols_proj_padded, num_rows,
                                                              proj_padded_.dataPointer());
    GCT::CUDA::checkErrorKernel("Computation of padded projection");
}

template<size_t Dim>
uint RampFilteringFFT::numRows(std::array<unsigned, Dim> const &size)
{
    auto num_rows = 1u;
    if constexpr (Dim > 1u) {
        num_rows = std::accumulate(std::cbegin(size) + 1u,
                                   std::cend(size), 1u, std::multiplies<>());
    }
    return num_rows;
}

template<typename ProjFloatCUDev>
void RampFilteringFFT::runFiltering(ProjFloatCUDev &proj)
{
    auto const batch_size = numRows(proj.dimensions());
    if (batch_size == num_rows_filtered_) {
        setProjPadded(proj);
        FFT1D(proj_padded_, proj_padded_fft_);
        multFFT();
        FFT1DInv();
        extractProj(proj);
    }
}
}//GCT::Recon namespace.

#endif //RAMP_FILTERING_FFT_CUH
