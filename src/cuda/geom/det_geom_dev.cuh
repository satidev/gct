#ifndef DET_GEOM_DEV_CUH
#define DET_GEOM_DEV_CUH

#include "../../cpp/geom/detector_1d.h"
#include "../data/unified_mem.cuh"
#include "../data/copy_host_dev.cuh"
#include <string>

namespace GCT::Geom::CUDA
{
// Detector geometry parameters are stored in CUDA device.
template<typename DetRow, typename ...DetCol>
struct DetGeomDev;

// Specialization for 1D detectors.
template<typename DetRow>
struct DetGeomDev<DetRow>
{
public:
    explicit DetGeomDev(std::tuple<DetRow> const &dg)
        :
        det_row{1u}
    {
        try {
            auto const dr = std::get<0>(dg);
            det_row.dataPointer()->sampler.num_samples = dr.sampler.num_samples;
            det_row.dataPointer()->sampler.sampling_width = dr.sampler.sampling_width;
        }
        catch (std::exception const &e) {
            throw std::runtime_error{
                "Error during host to device transfer of detector geometry.\n" +
                    std::string{e.what()}};
        }
    }
    DetRow const *detRow() const noexcept
    {
        return det_row.dataPointer();
    }

private:
    Data::CUDA::UnifiedMem<DetRow> det_row;
};

// Specialization for 2D detectors.
template<typename DetRow, typename DetCol>
struct DetGeomDev<DetRow, DetCol>
{
public:
    explicit DetGeomDev(std::tuple<DetRow, DetCol> const &dg)
        :
        det_row{1u}, det_col{1u}
    {
        try {
            auto const &[dr, dc] = dg;
            det_row.dataPointer()->sampler.num_samples = dr.sampler.num_samples;
            det_row.dataPointer()->sampler.sampling_width = dr.sampler.sampling_width;
            det_col.dataPointer()->sampler.num_samples = dc.sampler.num_samples;
            det_col.dataPointer()->sampler.sampling_width = dc.sampler.sampling_width;
        }
        catch (std::exception const &e) {
            throw std::runtime_error{
                "Error during host to device transfer of detector geometry.\n" +
                    std::string{e.what()}};
        }
    }
    DetRow const *detRow() const noexcept
    {
        return det_row.dataPointer();
    }
    DetCol const *detCol() const noexcept
    {
        return det_col.dataPointer();
    }

private:
    Data::CUDA::UnifiedMem<DetRow> det_row;
    Data::CUDA::UnifiedMem<DetCol> det_col;
};

}//GCT::Geom::CUDA namespace.

#endif //DET_GEOM_DEV_CUH
