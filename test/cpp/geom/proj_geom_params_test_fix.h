#ifndef PROJ_GEOM_PARAMS_TEST_FIX_H
#define PROJ_GEOM_PARAMS_TEST_FIX_H

#include "../../../src/cpp/io/parameters/proj_geom_params.h"
#include "../../../src/cpp/geom/detector_1d.h"
#include "../../../src/cpp/geom/proj_geom.h"
#include <numbers>

namespace GCT::Test
{
struct ProjGeomTestParams
{
protected:
    ProjGeomTestParams()
    {
        rot.dir = GCT::Geom::RotDir::CW;
        using namespace GCT::Geom::Units;
        rot.init_ang = Radians{std::numbers::pi_v<float> / 2.0f};
        rot.ang_incr = Radians{std::numbers::pi_v<float> / 2.0f};
        rot.num_proj = 4u;
        rot.hel_pitch = 0.0f;
    }
    GCT::Geom::RotationParams rot;
    float eps = 0.0001f;
};

struct Parallel1DTestParams: public ProjGeomTestParams
{
public:
    Parallel1DTestParams()
    {
        params.rot = rot;
        params.wcs = Geom::WCS::DCM;
        using namespace Geom::Units;
        params.det_row_center = 256.0_pix;
    }
    Geom::Parallel1DProjGeomParams getParams() const noexcept{
        return params;
    }
protected:
    Geom::Parallel1DProjGeomParams params;
};

struct FanBeamTestParams: public ProjGeomTestParams
{
public:
    FanBeamTestParams()
    {
        params.rot = rot;
        params.wcs = Geom::WCS::DCM;
        using namespace Geom::Units;
        params.det_row_center = 256.0_pix;
        using namespace Geom::Units;
        params.src_iso_dist = 500.0_mm;
        params.src_det_dist = 1000.0_mm;
    }
    Geom::FanBeamProjGeomParams getParams() const noexcept{
        return params;
    }
protected:
    Geom::FanBeamProjGeomParams params;
};

struct Parallel2DTestParams: public ProjGeomTestParams
{
public:
    Parallel2DTestParams()
    {
        params.rot = rot;
        params.wcs = Geom::WCS::DCM;
        using namespace Geom::Units;
        params.det_row_center = 256.0_pix;
        params.det_col_center = 257.0_pix;
    }
    Geom::Parallel2DProjGeomParams getParams() const noexcept{
        return params;
    }
protected:
    Geom::Parallel2DProjGeomParams params;
};

struct ConeBeamTestParams: public ProjGeomTestParams
{
public:
    ConeBeamTestParams()
    {
        params.rot = rot;
        params.wcs = Geom::WCS::DCM;
        using namespace Geom::Units;
        params.det_row_center = 256.0_pix;
        params.det_col_center = 257.0_pix;
        using namespace Geom::Units;
        params.src_iso_dist = 500.0_mm;
        params.src_det_dist = 1000.0_mm;
    }
    Geom::ConeBeamProjGeomParams getParams() const noexcept{
        return params;
    }
protected:
    Geom::ConeBeamProjGeomParams params;
};

template<typename DetRow, typename ...DetCol>
struct ProjGeomParamsTypeFactory;

template<>
struct ProjGeomParamsTypeFactory<Geom::DetParallel>
{
    typedef Parallel1DTestParams type;
};

template<>
struct ProjGeomParamsTypeFactory<Geom::DetFanArc>
{
    typedef FanBeamTestParams type;
};

template<>
struct ProjGeomParamsTypeFactory<Geom::DetFanLine>
{
    typedef FanBeamTestParams type;
};

template<>
struct ProjGeomParamsTypeFactory<Geom::DetParallel, Geom::DetParallel>
{
    typedef Parallel2DTestParams type;
};


template<>
struct ProjGeomParamsTypeFactory<Geom::DetParallel, Geom::DetFanLine>
{
    typedef ConeBeamTestParams type;
};

template<>
struct ProjGeomParamsTypeFactory<Geom::DetFanArc, Geom::DetFanLine>
{
    typedef ConeBeamTestParams type;
};

template<>
struct ProjGeomParamsTypeFactory<Geom::DetFanLine, Geom::DetFanLine>
{
    typedef ConeBeamTestParams type;
};

}//GCT::Test namespace.
#endif //PROJ_GEOM_PARAMS_TEST_FIX_H
