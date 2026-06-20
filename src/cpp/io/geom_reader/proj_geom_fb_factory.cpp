#include "proj_geom_fb_factory.h"
#include "proj_geom_cb_factory.h"

namespace GCT::Geom::ProjGeomFBFactory
{
ProjGeomFanBeam create(FanBeamProjGeomParams const &params, unsigned proj_idx)
{
    auto const rot_ang_rad = params.rot.init_ang +
        params.rot.ang_incr * static_cast<float>(proj_idx);
    return create(params, rot_ang_rad);
}

ProjGeomFanBeam create(FanBeamProjGeomParams const &params, Units::Radians rot_ang)
{
    auto cb_params = ConeBeamProjGeomParams{};
    cb_params.src_iso_dist = params.src_iso_dist;
    cb_params.src_det_dist = params.src_det_dist;
    cb_params.det_row_center = params.det_row_center;
    cb_params.det_col_center = params.det_row_center;
    cb_params.wcs = params.wcs;
    cb_params.rot.dir = params.rot.dir;
    cb_params.rot.init_ang = params.rot.init_ang;
    cb_params.rot.ang_incr = params.rot.ang_incr;
    cb_params.rot.num_proj = params.rot.num_proj;

    auto const cb_geom = ProjGeomCBFactory::create(cb_params, rot_ang);

    ProjGeomFanBeam fb_geom{};

    fb_geom.src_det_dist = cb_geom.src_det_dist;
    fb_geom.src_iso_dist = cb_geom.src_iso_dist;
    fb_geom.det_row_center = cb_geom.det_row_center;
    fb_geom.rot_ang = cb_geom.rot_ang;

    fb_geom.src_pos.x = cb_geom.src_pos.x;
    fb_geom.src_pos.y = cb_geom.src_pos.y;

    fb_geom.det_row_dir.x = cb_geom.det_row_dir.x;
    fb_geom.det_row_dir.y = cb_geom.det_row_dir.y;

    fb_geom.src_det_dir.x = cb_geom.src_det_dir.x;
    fb_geom.src_det_dir.y = cb_geom.src_det_dir.y;

    return fb_geom;
}

}//GCT::Geom::ProjGeomFBFactory namespace.



