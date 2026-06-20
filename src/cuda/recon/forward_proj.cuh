#ifndef GCT_FORWARD_PROJ_CUH
#define GCT_FORWARD_PROJ_CUH

#include "../geom/ct_geom_dev.cuh"
#include <iostream>
#include "../geom/coord_trans_proj.cuh"
#include "../numeric/vec2d_math.cuh"
#include "../data/dev_mem.cuh"
#include "../geom/coord_trans_proj.cuh"
#include "../geom/proj_geom_dev.cuh"

namespace GCT::Recon::CUDA::ForwardProjDev
{
__global__ void forwardProjKern(Geom::DetParallel const *det_row,
                                Geom::CUDA::ProjGeomScanDev<Geom::DetParallel> const *pg,
                                cudaTextureObject_t img_tex,
                                Geom::ReconImgGeom<2u> img_geom,
                                unsigned num_proj, float *proj)
{

    auto const col_idx_proj = blockIdx.x * blockDim.x + threadIdx.x;
    auto const proj_idx = blockIdx.y * blockDim.y + threadIdx.y;

    auto const num_cols_proj = det_row->sampler.num_samples;
    if (col_idx_proj < num_cols_proj && proj_idx < num_proj) {

        //Convert pixel coordinates to detector coordinates.
        using namespace Geom::CoordTransProj;
        using namespace Geom::CoordTransImg;

        auto const num_cols_img = img_geom.x.num_samples;
        auto const num_rows_img = img_geom.y.num_samples;
        auto const pix_width_mm = img_geom.x.sampling_width;
        auto const pix_height_mm = img_geom.y.sampling_width;
        auto const img_width_mm = static_cast<float>(num_cols_img) * pix_width_mm;
        auto const img_height_mm = static_cast<float>(num_rows_img) * pix_height_mm;

        using namespace GCT::Geom::Vec2DMath::CUDA;//For vector arithmetic and norm.

        //We consider an array of hypothetical X-ray sources and detector bins are
        //arranged covering the image.
        //The diagonal of image box is considered as the source-detector distance.
        //An extra image pixels are considered to avoid numerical errors.
        auto const src_det_dist_mm = norm(make_float2(img_width_mm + 3.0f * pix_width_mm,
                                                      img_height_mm + 3.0f * pix_width_mm));

        auto const rot_ang_rad = pg->rot_ang_rad[proj_idx];
        //2D direction (in the world coordinate system) of parallel rays,
        //from the detector bins to the X-ray sources.
        auto const ray_dir_wcs = make_float2(cos(rot_ang_rad), sin(rot_ang_rad));

        //Detector center where the world origin is projected.
        auto const det_cent_pos_wcs = ray_dir_wcs * (-src_det_dist_mm / 2.0f);

        //Detector direction can be computed by the 90 degree of clockwise rotation
        //of parallel ray direction.
        auto const det_row_dir = make_float2(-sin(rot_ang_rad), cos(rot_ang_rad));

        //Transform indices to detector coordinate system
        auto const det_coord_row = PCSToDCS(col_idx_proj, det_row->sampler.sampling_width,
                                            pg->det_row_center[proj_idx]);
        //2D position vector of detector bin/pixel in world coordinate system.
        auto const det_bin_pos_wcs = det_cent_pos_wcs + det_row_dir * det_coord_row;

        //For constant step length projector.
        auto const step_length_mm = min(pix_width_mm, pix_height_mm);
        auto const num_steps = static_cast<unsigned >(src_det_dist_mm / step_length_mm);
        auto const step_vec_wcs = ray_dir_wcs * step_length_mm;

        auto const first_pix_pos = firstPixPos(img_geom);

        auto ray_sum = 0.0f;
        for (auto step_idx = 0u; step_idx < num_steps; step_idx++) {

            //Image pixel position in world coordinate system.
            auto const pix_pos_wcs = det_bin_pos_wcs +
                step_vec_wcs * static_cast<float>(step_idx);
            //Transformation from world coordinate system to image coordinate system.
            auto const pix_pos_ics = (pix_pos_wcs - first_pix_pos) /
                make_float2(pix_width_mm, pix_height_mm);

            ray_sum += tex2D<float>(img_tex, pix_pos_ics.x + 0.5f, pix_pos_ics.y + 0.5f);
        }

        auto const arr_pos_proj = proj_idx * num_cols_proj + col_idx_proj;
        proj[arr_pos_proj] = ray_sum;
    }
}

template<typename DetRow>
void computeProj(Geom::CUDA::CTGeomDev<DetRow> const &ct_geom,
                 Data::CUDA::NDArrayTexture<float, 2u> const &img,
                 Geom::ReconImgGeom<2u> const &img_geom,
                 Data::CUDA::ProjectionStack<float, 1u, Data::CUDA::DevMem<float>> &proj)
{
    auto const config = GCT::CUDA::execConfig(proj.dimensions());
    forwardProjKern<<<config.first, config.second>>>(ct_geom.detGeom().detRow(),
                                                     ct_geom.projGeom(),
                                                     img.texObject(),
                                                     img_geom,
                                                     ct_geom.numProj(),
                                                     proj.dataPointer());
}
}//GCT::Recon::CUDA::ForwardProjDev namespace.

#endif //GCT_FORWARD_PROJ_CUH
