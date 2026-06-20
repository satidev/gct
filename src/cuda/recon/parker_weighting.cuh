#ifndef PARKER_WEIGHTING_CUH
#define PARKER_WEIGHTING_CUH

#include "../../cuda/data/vector_dev.cuh"
#include "../utils/cuda_config.cuh"
#include "../data/projection.cuh"
#include "../data/copy_host_dev.cuh"
#include "../data/unified_mem.cuh"
#include "../data/array_2d_dev.cuh"
#include "../data/projection_stack.cuh"
#include "../data/dev_mem.cuh"
#include "../numeric/constants.cuh"
#include "../../cpp/geom/ct_geom.h"

namespace GCT::Recon::CUDA
{
namespace Detail
{
//Computes the fan angle associated with the pixel of arc-shaped detector.
template<typename ...DetCol>
float fanAngle(Geom::DetFanArc const &dr,
               Geom::ProjGeom<Geom::DetFanArc, DetCol...> const &pg,
               unsigned det_col_idx)
{
    //Fan angle with respect to the iso-ray.
    return (static_cast<float>(det_col_idx) - pg.det_row_center.val)
        * dr.sampler.sampling_width.val;
}

//Computes the fan angle associated with the pixel of line-shaped detector.
template<typename ...DetCol>
float fanAngle(Geom::DetFanLine const &dr,
               Geom::ProjGeom<Geom::DetFanLine, DetCol...> const &pg,
               unsigned det_col_idx)
{
    float const det_coord = (static_cast<float>(det_col_idx) - pg.det_row_center.val) *
        dr.sampler.sampling_width.val;
    //Computes the fan angle with respect to the principal ray.
    return atan2(det_coord, pg.src_det_dist.val);
}

//Fan angle covering the entire arc-shaped detector.
template<typename ...DetCol>
float fanAngleDet(Geom::DetFanArc const &dr,
                  Geom::ProjGeom<Geom::DetFanArc, DetCol...> const &)
{
    return static_cast<float>(dr.sampler.num_samples) * dr.sampler.sampling_width.val;
}

//Fan angle covering the entire line-shaped detector.
template<typename ...DetCol>
float fanAngleDet(Geom::DetFanLine const &dr,
                  Geom::ProjGeom<Geom::DetFanLine, DetCol...> const &pg)
{
    auto const tot_det_length_mm = static_cast<float>(dr.sampler.num_samples) *
        dr.sampler.sampling_width.val;
    return 2.0f * atan2(tot_det_length_mm / 2.0f, pg.src_det_dist.val);
}

//Compute fan angles of all detector pixels.
template<typename DetRow, typename ...DetCol>
std::vector<float> fanAngles(Geom::CTGeom<DetRow, DetCol...> const &cg)
{
    auto const det_row = std::get<0>(cg.detGeom());
    auto indices = std::vector<uint>(det_row.sampler.num_samples);
    std::iota(std::begin(indices), std::end(indices), 0u);

    auto fan_ang_rad = std::vector<float>{};
    auto const pg = cg.projGeom().front();
    std::transform(std::cbegin(indices), std::cend(indices), std::back_inserter(fan_ang_rad),
                   [&det_row, &pg](unsigned idx)
                   {
                       return fanAngle(det_row, pg, idx);
                   }
    );
    return fan_ang_rad;
}

//Parker weight associated with the fan angle.
__device__ __host__ inline float parkerWeightFan(float fan_ang_rad, float det_fan_ang_rad,
                                 float rel_rot_ang_rad, float rot_ang_range_rad);
__device__ __host__ inline float parkerWeightFan(float fan_ang_rad, float det_fan_ang_rad,
                                 float rel_rot_ang_rad, float rot_ang_range_rad)
{
    auto weight = 0.0f;
    //Compute end points of angular intervals.
    auto const PI_F = Numeric::Constants::CUDA::pi();
    auto const t1 = 0.0f;
    auto const t2 = 2 * fan_ang_rad + rot_ang_range_rad - PI_F;
    auto const t3 = 2 * fan_ang_rad + PI_F;
    auto const t4 = rot_ang_range_rad;
    //Compute parker weights for each interval.
    if ((rel_rot_ang_rad >= t1) && (rel_rot_ang_rad <= t2)) {
        weight = powf(sinf(PI_F * rel_rot_ang_rad /
            (2.0f * (rot_ang_range_rad - PI_F) + 4.0f * fan_ang_rad)), 2.0f);
    }
    else if ((rel_rot_ang_rad >= t2) && (rel_rot_ang_rad <= t3)) {
        weight = 1.0f;
    }
    else if ((rel_rot_ang_rad >= t3) && (rel_rot_ang_rad <= t4)) {
        weight = powf(sinf(PI_F * (rot_ang_range_rad - rel_rot_ang_rad) /
            (2.0f * (rot_ang_range_rad - PI_F) - 4.0f * fan_ang_rad)), 2.0f);
    }
    return weight;
}



__global__ void parkerWeightKern(float const *fan_angles, float const *rel_rot_angles,
                                 float det_fan_ang_rad, float rot_ang_range_rad,
                                 uint num_cols, uint num_proj, float *weight_img);




__global__ void parkerWeightingSingleProjKernel(float const *weight_img,
                                                unsigned num_det_cols,
                                                unsigned num_det_rows,
                                                unsigned num_proj,
                                                unsigned proj_idx,
                                                float *proj);


__global__ void parkerWeightingAllProjKernel(float const *weight_img,
                                             unsigned num_det_cols,
                                             unsigned num_det_rows,
                                             unsigned num_proj,
                                             float *proj);

}//Detail namespace.

template<typename DetRow, typename ...DetCol>
Data::CUDA::Array2DDev<float> getParkerWeights(Geom::CTGeom<DetRow, DetCol...> const &cg)
{
    auto const num_cols = std::get<0>(cg.detGeom()).sampler.num_samples;
    auto const num_proj = static_cast<unsigned>(cg.projGeom().size());

    auto const fan_angles = Detail::fanAngles(cg);
    auto const det_fan_angle = Detail::fanAngleDet(
        std::get<0>(cg.detGeom()), cg.projGeom().front());

    auto rel_rot_angles = std::vector<float>{};
    rel_rot_angles.reserve(cg.rotationGeom().rel_rot_ang.size());
    for(auto const &ang:cg.rotationGeom().rel_rot_ang)
        rel_rot_angles.push_back(ang.val);

    if(cg.rotationGeom().dir == Geom::RotDir::CCW){
        std::reverse(std::begin(rel_rot_angles), std::end(rel_rot_angles));
    }
    //auto const rel_rot_angles = cg.rotationGeom().rel_rot_ang;
    auto const rot_ang_range = cg.rotationGeom().rot_ang_range;
    auto const config = GCT::CUDA::execConfig(
        std::array<unsigned, 2u>{num_cols, num_proj});

    typedef Data::CUDA::VectorDev<float> VecFloat;

    auto fan_angles_dev = VecFloat{static_cast<unsigned >(fan_angles.size())};
    Data::CUDA::copyHostToDev(fan_angles, fan_angles_dev);

    auto rot_angles_dev = VecFloat{static_cast<unsigned >(rel_rot_angles.size())};
    Data::CUDA::copyHostToDev(rel_rot_angles, rot_angles_dev);

    auto weights = Data::CUDA::Array2DDev<float, Data::CUDA::DevMem<float>>{num_cols, num_proj};
    if (rot_ang_range > Geom::Units::Radians{std::numbers::pi_v<float>}) {
        Detail::parkerWeightKern<<<config.first, config.second>>>(fan_angles_dev.dataPointer(),
            rot_angles_dev.dataPointer(),
            det_fan_angle, rot_ang_range.val,
            num_cols, num_proj,
            weights.dataPointer());
        auto const status = cudaGetLastError();
        if (status != cudaSuccess) {
            throw std::runtime_error{"Error while executing Parker weights kernel.\n"};
        }
    }
    return weights;
}

// Specialized function for parallel beam geometry.
// Currently weights of 0.5 (half-scan) and 1.0 (full-scan) are considered.
// TODO:Implement weighting scheme considering the redundancy of 2D Radon transform.
template<typename ...DetCol>
Data::CUDA::Array2DDev<float> getParkerWeights(Geom::CTGeom<Geom::DetParallel, DetCol...> const &cg)
{
    auto const num_cols = std::get<0>(cg.detGeom()).sampler.num_samples;
    auto const num_proj = static_cast<unsigned >(cg.projGeom().size());

    auto const rel_rot_angles = cg.rotationGeom().rel_rot_ang;
    auto const rot_ang_incr = cg.rotationGeom().rot_ang_incr;
    auto const rot_ang_range = cg.rotationGeom().rot_ang_range;

    auto weights = Data::CUDA::Array2DDev<float>{num_cols, num_proj};
    if (rot_ang_incr > Geom::Units::Radians{0.0f} &&
        rot_ang_range <= (rot_ang_incr + std::numbers::pi_v<float>)) {
        weights.setVal(1.0f);
    }
    else {
        weights.setVal(0.5f);
    }
    return weights;
}

template<typename ProjCUDevType>
void runParkerWeighting(Data::CUDA::Array2DDev<float> const &weight_img,
                        unsigned proj_idx, ProjCUDevType &proj)
{
    auto const config = GCT::CUDA::execConfig(std::array<unsigned, 2u>{proj.projSize().num_cols,
                                                                       Data::numProjRows(proj.projSize())});
    Detail::parkerWeightingSingleProjKernel<<<config.first, config.second>>>(
        weight_img.dataPointer(),
            proj.projSize().num_cols,
            Data::numProjRows(proj.projSize()),
            weight_img.numRows(),
            proj_idx, proj.dataPointer());
    GCT::CUDA::checkErrorKernel("Parker weighting (single projection)");
}

template<typename ProjStackCUDevType>
void runParkerWeighting(Data::CUDA::Array2DDev<float> const &weight_img,
                        ProjStackCUDevType &proj)
{
    auto const num_proj = weight_img.numRows();
    auto const config = GCT::CUDA::execConfig(std::array<unsigned, 3u>{proj.projSize().num_cols,
                                                                       Data::numProjRows(proj.projSize()),
                                                                       num_proj});
    Detail::parkerWeightingAllProjKernel<<<config.first, config.second>>>(
        weight_img.dataPointer(),
            proj.projSize().num_cols, Data::numProjRows(proj.projSize()),
            num_proj, proj.dataPointer());
    GCT::CUDA::checkErrorKernel("Parker weighting (all projections)");
}
}//GCT::Recon::CUDA namespace.

#endif //PARKER_WEIGHTING_CUH
