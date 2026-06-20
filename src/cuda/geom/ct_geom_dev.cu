#include "ct_geom_dev.cuh"
#include "../../cpp/geom/projection_matrix.h"
#include "../../cpp/geom/geom_math.h"

namespace GCT::Geom::CUDA
{
template<>
void CTGeomDev<DetParallel>::allocateProjGeom()
{
    auto const func_desc = std::string{"allocation of 1D parallel projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->rot_ang_rad),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
}

template<>
void CTGeomDev<DetParallel>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->rot_ang_rad);
    cudaFree(pg_scan.dataPointer()->det_row_center);
}

template<>
void CTGeomDev<DetParallel>::copyProjGeomHostToDev(std::vector<ProjGeom<DetParallel>> const &host)
{
    auto proj_idx = size_t{0};
    for (auto const &pg: host) {
        pg_scan.dataPointer()->rot_ang_rad[proj_idx] = pg.rot_ang.val;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;
        proj_idx++;
    }
}

template<>
void CTGeomDev<DetFanArc>::allocateProjGeom()
{
    auto const func_desc = std::string{
        "allocation of fan-beam (arc) projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_det_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_iso_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    //GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.buffPtr()->rot_ang_rad),
    //                                        num_proj * sizeof(float)),
    //                      func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_pos),
                                            num_proj * sizeof(float2)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_dir),
                                            num_proj * sizeof(float2)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->iso_ray_dir),
                                            num_proj * sizeof(float2)),
                          func_desc);
}

template<>
void CTGeomDev<DetFanArc>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->src_det_dist_mm);
    cudaFree(pg_scan.dataPointer()->src_iso_dist_mm);
    //cudaFree(pg_scan.buffPtr()->rot_ang_rad);
    cudaFree(pg_scan.dataPointer()->det_row_center);
    cudaFree(pg_scan.dataPointer()->src_pos);
    cudaFree(pg_scan.dataPointer()->det_row_dir);
    cudaFree(pg_scan.dataPointer()->iso_ray_dir);
}

template<>
void CTGeomDev<DetFanArc>::copyProjGeomHostToDev(std::vector<ProjGeom<DetFanArc>> const &host)
{
    auto proj_idx = 0u;
    for (auto const &pg: host) {

        pg_scan.dataPointer()->src_det_dist_mm[proj_idx] = pg.src_det_dist.val;
        pg_scan.dataPointer()->src_iso_dist_mm[proj_idx] = Geom::Math::norm(pg.src_pos);
        //pg_scan.buffPtr()->rot_ang_rad[proj_idx] = pg.rot_ang_rad;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;

        pg_scan.dataPointer()->src_pos[proj_idx].x = pg.src_pos.x.val;
        pg_scan.dataPointer()->src_pos[proj_idx].y = pg.src_pos.y.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].x = pg.det_row_dir.x.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].y = pg.det_row_dir.y.val;
        pg_scan.dataPointer()->iso_ray_dir[proj_idx].x = pg.iso_ray_dir.x.val;
        pg_scan.dataPointer()->iso_ray_dir[proj_idx].y = pg.iso_ray_dir.y.val;
        proj_idx++;
    }
}

template<>
void CTGeomDev<DetFanLine>::allocateProjGeom()
{
    auto const func_desc = std::string{
        "allocation of fan-beam (line) projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_det_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_iso_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    //GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.buffPtr()->rot_ang_rad),
    //                                        num_proj * sizeof(float)),
    //                      func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_pos),
                                            num_proj * sizeof(float2)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_dir),
                                            num_proj * sizeof(float2)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->iso_ray_dir),
                                            num_proj * sizeof(float2)),
                          func_desc);
}

template<>
void CTGeomDev<DetFanLine>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->src_det_dist_mm);
    cudaFree(pg_scan.dataPointer()->src_iso_dist_mm);
    //cudaFree(pg_scan.buffPtr()->rot_ang_rad);
    cudaFree(pg_scan.dataPointer()->det_row_center);
    cudaFree(pg_scan.dataPointer()->src_pos);
    cudaFree(pg_scan.dataPointer()->det_row_dir);
    cudaFree(pg_scan.dataPointer()->iso_ray_dir);
}

template<>
void CTGeomDev<DetFanLine>::copyProjGeomHostToDev(std::vector<ProjGeom<DetFanLine>> const &host)
{
    auto proj_idx = 0u;
    for (auto const &pg: host) {

        pg_scan.dataPointer()->src_det_dist_mm[proj_idx] = pg.src_det_dist.val;
        pg_scan.dataPointer()->src_iso_dist_mm[proj_idx] = Geom::Math::norm(pg.src_pos);
        //pg_scan.buffPtr()->rot_ang_rad[proj_idx] = pg.rot_ang_rad;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;

        pg_scan.dataPointer()->src_pos[proj_idx].x = pg.src_pos.x.val;
        pg_scan.dataPointer()->src_pos[proj_idx].y = pg.src_pos.y.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].x = pg.det_row_dir.x.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].y = pg.det_row_dir.y.val;

        auto const src_det_dir = Math::rot90(pg.det_row_dir);
        pg_scan.dataPointer()->iso_ray_dir[proj_idx].x = src_det_dir.x.val;
        pg_scan.dataPointer()->iso_ray_dir[proj_idx].y = src_det_dir.y.val;
        proj_idx++;
    }
}

template<>
void CTGeomDev<DetParallel, DetParallel>::allocateProjGeom()
{
    auto const func_desc = std::string{"allocation of 2D parallel projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->rot_ang_rad),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_center),
                                            num_proj * sizeof(float)),
                          func_desc);
}

template<>
void CTGeomDev<DetParallel, DetParallel>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->rot_ang_rad);
    cudaFree(pg_scan.dataPointer()->det_row_center);
    cudaFree(pg_scan.dataPointer()->det_col_center);
}

template<>
void CTGeomDev<DetParallel, DetParallel>::copyProjGeomHostToDev(
    std::vector<ProjGeom<DetParallel, DetParallel>> const &host)
{
    auto proj_idx = size_t{0};
    for (auto const &pg: host) {
        pg_scan.dataPointer()->rot_ang_rad[proj_idx] = pg.rot_ang.val;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;
        pg_scan.dataPointer()->det_col_center[proj_idx] = pg.det_col_center.val;
        proj_idx++;
    }
}

template<>
void CTGeomDev<DetParallel, DetFanLine>::allocateProjGeom()
{
    auto const func_desc = std::string{"allocation of 2D parallel projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_det_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_iso_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->rot_ang_rad),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_center),
                                            num_proj * sizeof(float)),
                          func_desc);
}

template<>
void CTGeomDev<DetParallel, DetFanLine>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->src_det_dist_mm);
    cudaFree(pg_scan.dataPointer()->src_iso_dist_mm);
    cudaFree(pg_scan.dataPointer()->rot_ang_rad);
    cudaFree(pg_scan.dataPointer()->det_row_center);
    cudaFree(pg_scan.dataPointer()->det_col_center);
}

template<>
void CTGeomDev<DetParallel, DetFanLine>::copyProjGeomHostToDev(
    std::vector<ProjGeom<DetParallel, DetFanLine>> const &host)
{
    auto proj_idx = size_t{0};
    for (auto const &pg: host) {
        pg_scan.dataPointer()->src_det_dist_mm[proj_idx] = pg.src_det_dist.val;
        pg_scan.dataPointer()->src_iso_dist_mm[proj_idx] = pg.src_iso_dist.val;
        pg_scan.dataPointer()->rot_ang_rad[proj_idx] = pg.rot_ang.val;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;
        pg_scan.dataPointer()->det_col_center[proj_idx] = pg.det_col_center.val;
        proj_idx++;
    }
}

template<>
void CTGeomDev<DetFanArc, DetFanLine>::allocateProjGeom()
{
    auto const func_desc = std::string{
        "allocation of cylindrical detector projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_det_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_iso_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    //GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.buffPtr()->rot_ang_rad),
    //                                        num_proj * sizeof(float)),
    //                      func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_pos),
                                            num_proj * sizeof(float3)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->iso_ray_dir),
                                            num_proj * sizeof(float3)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_dir),
                                            num_proj * sizeof(float3)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_dir),
                                            num_proj * sizeof(float3)),
                          func_desc);
}

template<>
void CTGeomDev<DetFanArc, DetFanLine>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->src_det_dist_mm);
    cudaFree(pg_scan.dataPointer()->src_iso_dist_mm);
    //cudaFree(pg_scan.buffPtr()->rot_ang_rad);
    cudaFree(pg_scan.dataPointer()->det_row_center);
    cudaFree(pg_scan.dataPointer()->det_col_center);
    cudaFree(pg_scan.dataPointer()->src_pos);
    cudaFree(pg_scan.dataPointer()->iso_ray_dir);
    cudaFree(pg_scan.dataPointer()->det_row_dir);
    cudaFree(pg_scan.dataPointer()->det_col_dir);
}

template<>
void CTGeomDev<DetFanArc, DetFanLine>::copyProjGeomHostToDev(
    std::vector<ProjGeom<DetFanArc, DetFanLine>> const &host)
{
    auto proj_idx = size_t{0};
    for (auto const &pg: host) {

        pg_scan.dataPointer()->src_det_dist_mm[proj_idx] = pg.src_det_dist.val;
        pg_scan.dataPointer()->src_iso_dist_mm[proj_idx] = Geom::Math::norm(pg.src_pos);
        //pg_scan.buffPtr()->rot_ang_rad[proj_idx] = pg.rot_ang_rad;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;
        pg_scan.dataPointer()->det_col_center[proj_idx] = pg.det_col_center.val;

        pg_scan.dataPointer()->src_pos[proj_idx].x = pg.src_pos.x.val;
        pg_scan.dataPointer()->src_pos[proj_idx].y = pg.src_pos.y.val;
        pg_scan.dataPointer()->src_pos[proj_idx].z = pg.src_pos.z.val;

        pg_scan.dataPointer()->iso_ray_dir[proj_idx].x = pg.iso_ray_dir.x.val;
        pg_scan.dataPointer()->iso_ray_dir[proj_idx].y = pg.iso_ray_dir.y.val;
        pg_scan.dataPointer()->iso_ray_dir[proj_idx].z = pg.iso_ray_dir.z.val;

        pg_scan.dataPointer()->det_row_dir[proj_idx].x = pg.det_row_dir.x.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].y = pg.det_row_dir.y.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].z = pg.det_row_dir.z.val;

        pg_scan.dataPointer()->det_col_dir[proj_idx].x = pg.det_col_dir.x.val;
        pg_scan.dataPointer()->det_col_dir[proj_idx].y = pg.det_col_dir.y.val;
        pg_scan.dataPointer()->det_col_dir[proj_idx].z = pg.det_col_dir.z.val;

        proj_idx++;
    }
}

template<>
void CTGeomDev<DetFanLine, DetFanLine>::allocateProjGeom()
{
    auto const func_desc = std::string{
        "allocation of flat detector projection geometry parameters"};
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_det_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_iso_dist_mm),
                                            num_proj * sizeof(float)),
                          func_desc);
    //GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.buffPtr()->rot_ang_rad),
    //                                        num_proj * sizeof(float)),
    //                      func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_center),
                                            num_proj * sizeof(float)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_pos),
                                            num_proj * sizeof(float3)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->src_det_dir),
                                            num_proj * sizeof(float3)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_row_dir),
                                            num_proj * sizeof(float3)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->det_col_dir),
                                            num_proj * sizeof(float3)),
                          func_desc);

    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->proj_mat_row_1),
                                            num_proj * sizeof(float4)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->proj_mat_row_2),
                                            num_proj * sizeof(float4)),
                          func_desc);
    GCT::CUDA::checkError(cudaMallocManaged((void **) &(pg_scan.dataPointer()->proj_mat_row_3),
                                            num_proj * sizeof(float4)),
                          func_desc);
}

template<>
void CTGeomDev<DetFanLine, DetFanLine>::deallocateProjGeom()
{
    cudaFree(pg_scan.dataPointer()->src_det_dist_mm);
    cudaFree(pg_scan.dataPointer()->src_iso_dist_mm);
    cudaFree(pg_scan.dataPointer()->det_row_center);
    cudaFree(pg_scan.dataPointer()->det_col_center);
    cudaFree(pg_scan.dataPointer()->src_pos);
    cudaFree(pg_scan.dataPointer()->src_det_dir);
    cudaFree(pg_scan.dataPointer()->det_row_dir);
    cudaFree(pg_scan.dataPointer()->det_col_dir);
    cudaFree(pg_scan.dataPointer()->proj_mat_row_1);
    cudaFree(pg_scan.dataPointer()->proj_mat_row_2);
    cudaFree(pg_scan.dataPointer()->proj_mat_row_3);
}

template<>
void CTGeomDev<DetFanLine, DetFanLine>::copyProjGeomHostToDev(
    std::vector<ProjGeom<DetFanLine, DetFanLine>> const &host)
{
    auto proj_idx = size_t{0};
    for (auto const &pg: host) {

        pg_scan.dataPointer()->src_det_dist_mm[proj_idx] = pg.src_det_dist.val;
        pg_scan.dataPointer()->src_iso_dist_mm[proj_idx] = Geom::Math::norm(pg.src_pos);
        //pg_scan.buffPtr()->rot_ang_rad[proj_idx] = pg.rot_ang_rad;
        pg_scan.dataPointer()->det_row_center[proj_idx] = pg.det_row_center.val;
        pg_scan.dataPointer()->det_col_center[proj_idx] = pg.det_col_center.val;

        pg_scan.dataPointer()->src_pos[proj_idx].x = pg.src_pos.x.val;
        pg_scan.dataPointer()->src_pos[proj_idx].y = pg.src_pos.y.val;
        pg_scan.dataPointer()->src_pos[proj_idx].z = pg.src_pos.z.val;

        auto const src_det_dir = Math::crossProduct(pg.det_row_dir,
                                                    pg.det_col_dir);
        pg_scan.dataPointer()->src_det_dir[proj_idx].x = src_det_dir.x.val;
        pg_scan.dataPointer()->src_det_dir[proj_idx].y = src_det_dir.y.val;
        pg_scan.dataPointer()->src_det_dir[proj_idx].z = src_det_dir.z.val;

        pg_scan.dataPointer()->det_row_dir[proj_idx].x = pg.det_row_dir.x.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].y = pg.det_row_dir.y.val;
        pg_scan.dataPointer()->det_row_dir[proj_idx].z = pg.det_row_dir.z.val;

        pg_scan.dataPointer()->det_col_dir[proj_idx].x = pg.det_col_dir.x.val;
        pg_scan.dataPointer()->det_col_dir[proj_idx].y = pg.det_col_dir.y.val;
        pg_scan.dataPointer()->det_col_dir[proj_idx].z = pg.det_col_dir.z.val;

        auto const pix_width = dg.detRow()->sampler.sampling_width.val;
        auto const pix_height = dg.detCol()->sampler.sampling_width.val;
        auto const pmat = ProjectionMatrix{pg, pix_width, pix_height};
        auto const mat_elems = pmat.matrix();
        auto const mat_row_1 = mat_elems.row(0).cast<float>();
        pg_scan.dataPointer()->proj_mat_row_1[proj_idx].x = mat_row_1.x();
        pg_scan.dataPointer()->proj_mat_row_1[proj_idx].y = mat_row_1.y();
        pg_scan.dataPointer()->proj_mat_row_1[proj_idx].z = mat_row_1.z();
        pg_scan.dataPointer()->proj_mat_row_1[proj_idx].w = mat_row_1.w();

        auto const mat_row_2 = mat_elems.row(1).cast<float>();
        pg_scan.dataPointer()->proj_mat_row_2[proj_idx].x = mat_row_2.x();
        pg_scan.dataPointer()->proj_mat_row_2[proj_idx].y = mat_row_2.y();
        pg_scan.dataPointer()->proj_mat_row_2[proj_idx].z = mat_row_2.z();
        pg_scan.dataPointer()->proj_mat_row_2[proj_idx].w = mat_row_2.w();

        auto const mat_row_3 = mat_elems.row(2).cast<float>();
        pg_scan.dataPointer()->proj_mat_row_3[proj_idx].x = mat_row_3.x();
        pg_scan.dataPointer()->proj_mat_row_3[proj_idx].y = mat_row_3.y();
        pg_scan.dataPointer()->proj_mat_row_3[proj_idx].z = mat_row_3.z();
        pg_scan.dataPointer()->proj_mat_row_3[proj_idx].w = mat_row_3.w();

        proj_idx++;
    }
}

}//GCT::Geom::CUDA namespace.
