#include "ramp_filtering_fft.cuh"

namespace GCT::Recon
{
__device__ inline cufftComplex multComplex(cufftComplex a, cufftComplex b)
{
    auto res = cufftComplex{};
    res.x = a.x * b.x - a.y * b.y;
    res.y = a.x * b.y + a.y * b.x;
    return res;
}


__global__ void copyProjToPaddedProjKern(float const *proj,
                                         unsigned num_cols_proj,
                                         unsigned num_cols_proj_padded,
                                         unsigned num_rows,
                                         float *proj_padded)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_cols_proj) && (row_idx < num_rows)) {
        auto const pix_idx_proj = row_idx * num_cols_proj + col_idx;
        auto const pix_idx_proj_padded = row_idx * num_cols_proj_padded + col_idx;
        proj_padded[pix_idx_proj_padded] = proj[pix_idx_proj];
    }
}

__global__ void multFFTKern(cufftComplex const *filt,
                            unsigned num_cols,
                            unsigned num_rows,
                            cufftComplex *op)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_cols) && (row_idx < num_rows)) {
        auto const op_idx = row_idx * num_cols + col_idx;
        op[op_idx] = multComplex(op[op_idx], filt[col_idx]);
    }

}

__global__ void extractProjKern(float const *proj_filt,
                                unsigned num_cols_proj_filt,
                                unsigned num_cols_proj,
                                unsigned num_rows_proj,
                                float *proj)
{
    auto const col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    auto const row_idx = blockIdx.y * blockDim.y + threadIdx.y;

    if ((col_idx < num_cols_proj) && (row_idx < num_rows_proj)) {

        auto const col_idx_proj_filt = col_idx + (num_cols_proj / 2);
        auto const pix_idx_proj_filt = row_idx * num_cols_proj_filt + col_idx_proj_filt;
        auto const proj_pix_idx = row_idx * num_cols_proj + col_idx;

        proj[proj_pix_idx] = proj_filt[pix_idx_proj_filt] / static_cast<float>(num_cols_proj_filt);
    }
}

void RampFilteringFFT::multFFT()
{
    auto const config = CUDA::execConfig(proj_padded_fft_.dimensions());
    multFFTKern<<<config.first, config.second>>>(filt_padded_fft_.dataPointer(),
                                                 proj_padded_fft_.numCols(),
                                                 proj_padded_fft_.numRows(),
                                                 proj_padded_fft_.dataPointer());
    GCT::CUDA::checkErrorKernel("FFT multiplication (ramp filtering)");
}

void Recon::RampFilteringFFT::FFT1DInv()
{
    auto plan = cufftHandle{};
    auto const batch_size = static_cast<int>(num_rows_filtered_);
    auto rank = 1;
    auto num_samples = static_cast<int>(fft_len_);
    auto cufft_status = cufftPlanMany(&plan, rank, &num_samples,
                                      nullptr, 1, 0,
                                      nullptr, 1, 0,
                                      CUFFT_C2R, batch_size);
    if (cufft_status != CUFFT_SUCCESS) {
        throw std::runtime_error{std::string{"Error during inverse FFT computation (ramp filtering).\n"} +
            "Invalid FFT plan.\n"};
    }

    cufft_status = cufftExecC2R(plan, proj_padded_fft_.dataPointer(),
                                (cufftReal *) (proj_padded_.dataPointer()));
    if (cufft_status != CUFFT_SUCCESS) {
        throw std::runtime_error{std::string{"Error during inverse FFT computation (ramp filtering).\n"} +
            "Execution error.\n"};
    }
    cufftDestroy(plan);
}
}//GCT::Recon namespace.

