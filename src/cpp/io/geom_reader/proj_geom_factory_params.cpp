#include "proj_geom_factory.h"
#include "proj_geom_cb_factory.h"
#include "proj_geom_fb_factory.h"
#include "../parameters/proj_geom_params.h"

using namespace GCT::Geom;

template<>
template<>
ProjGeom<DetFanLine, DetFanLine> ProjGeomFactory<DetFanLine, DetFanLine>::create(
    ConeBeamProjGeomParams const &geom, unsigned proj_idx)
{
    auto const cb_pg = ProjGeomCBFactory::create(geom, proj_idx);
    auto pg = ProjGeom<DetFanLine, DetFanLine>{};
    pg.src_det_dist = cb_pg.src_det_dist;
    pg.det_row_center = cb_pg.det_row_center;
    pg.det_col_center = cb_pg.det_col_center;
    pg.src_pos = cb_pg.src_pos;
    pg.det_row_dir = cb_pg.det_row_dir;
    pg.det_col_dir = cb_pg.det_col_dir;
    return pg;
}

template<>
template<>
ProjGeom<DetFanArc, DetFanLine> ProjGeomFactory<DetFanArc, DetFanLine>::create(
    ConeBeamProjGeomParams const &geom, unsigned proj_idx)
{
    auto const cb_pg = ProjGeomCBFactory::create(geom, proj_idx);
    auto pg = ProjGeom<DetFanArc, DetFanLine>{};
    pg.src_det_dist = cb_pg.src_det_dist;
    pg.det_row_center = cb_pg.det_row_center;
    pg.det_col_center = cb_pg.det_col_center;
    pg.src_pos = cb_pg.src_pos;
    pg.det_row_dir = cb_pg.det_row_dir;
    pg.det_col_dir = cb_pg.det_col_dir;
    pg.iso_ray_dir = cb_pg.src_det_dir;
    return pg;
}

template<>
template<>
ProjGeom<DetParallel, DetParallel> ProjGeomFactory<DetParallel, DetParallel>::create(
    Parallel2DProjGeomParams const &geom, unsigned proj_idx)
{
    auto pg = ProjGeom<DetParallel, DetParallel>{};
    pg.det_row_center = geom.det_row_center;
    pg.det_col_center = geom.det_col_center;
    pg.rot_ang = geom.rot.init_ang +
        geom.rot.ang_incr * static_cast<float>(proj_idx);
    return pg;
}

template<>
template<>
ProjGeom<DetParallel, DetFanLine> ProjGeomFactory<DetParallel, DetFanLine>::create(
    ConeBeamProjGeomParams const &geom, unsigned proj_idx)
{
    auto pg = ProjGeom<DetParallel, DetFanLine>{};
    pg.src_det_dist = geom.src_det_dist;
    pg.src_iso_dist = geom.src_iso_dist;
    pg.det_row_center = geom.det_row_center;
    pg.det_col_center = geom.det_col_center;
    pg.rot_ang = geom.rot.init_ang +
        geom.rot.ang_incr * static_cast<float>(proj_idx);
    return pg;
}

template<>
template<>
ProjGeom<DetFanLine> ProjGeomFactory<DetFanLine>::create(
    FanBeamProjGeomParams const &geom, unsigned int proj_idx)
{
    auto const fb_pg = ProjGeomFBFactory::create(geom, proj_idx);
    auto pg = ProjGeom<DetFanLine>{};
    pg.src_det_dist = fb_pg.src_det_dist;
    pg.det_row_center = fb_pg.det_row_center;
    pg.src_pos = fb_pg.src_pos;
    pg.det_row_dir = fb_pg.det_row_dir;
    return pg;
}

template<>
template<>
ProjGeom<DetFanArc> ProjGeomFactory<DetFanArc>::create(
    FanBeamProjGeomParams const &geom, unsigned int proj_idx)
{
    auto const fb_pg = ProjGeomFBFactory::create(geom, proj_idx);
    auto pg = ProjGeom<DetFanArc>{};
    pg.src_det_dist = fb_pg.src_det_dist;
    pg.det_row_center = fb_pg.det_row_center;
    pg.src_pos = fb_pg.src_pos;
    pg.det_row_dir = fb_pg.det_row_dir;
    pg.iso_ray_dir = fb_pg.src_det_dir;
    return pg;
}

template<>
template<>
ProjGeom<DetParallel> ProjGeomFactory<DetParallel>::create(
    Parallel1DProjGeomParams const &geom, unsigned int proj_idx)
{
    auto pg = ProjGeom<DetParallel>{};
    pg.det_row_center = geom.det_row_center;
    pg.rot_ang = geom.rot.init_ang +
        geom.rot.ang_incr * static_cast<float>(proj_idx);
    return pg;
}





