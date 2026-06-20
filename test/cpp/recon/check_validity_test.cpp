#include "../../../thirdparty/catch.hpp"
#include "../../../src/cpp/recon/check_validity.h"
#include "../geom/ct_geom_test_fix.h"

namespace GCT::Test
{
struct Recon2DValidityTestFix
{
    Geom::CTGeom<Geom::DetParallel> cg = CTGeomTestFix<Geom::DetParallel>{}.ctGeom();
    Geom::DetParallel dr = std::get<0>(cg.detGeom());
    std::vector<Geom::ProjGeom<Geom::DetParallel>> pg = cg.projGeom();
};

struct Recon3DValidityTestFix
{
    Geom::CTGeom<Geom::DetFanArc, Geom::DetFanLine>
        cg = CTGeomTestFix<Geom::DetFanArc, Geom::DetFanLine>{}.ctGeom();
    Geom::DetFanArc dr = std::get<0>(cg.detGeom());
    Geom::DetFanLine dc = std::get<1>(cg.detGeom());
    std::vector<Geom::ProjGeom<Geom::DetFanArc, Geom::DetFanLine>> pg = cg.projGeom();
};


TEST_CASE_METHOD(Recon2DValidityTestFix,
    "Returns true since the 1D projection size and detector geometry are consistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<1u>{dr.sampler.num_samples};
    auto const num_proj = 10u;
    auto const proj = Data::ProjectionStack<float, 1u>{proj_size, num_proj};
    REQUIRE(Recon::Detail::checkProjDetConsistency(cg, proj, true));
}

TEST_CASE_METHOD(Recon3DValidityTestFix,
    "Returns true since the 2D projection size and detector geometry are consistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<2u>{dr.sampler.num_samples,
                                              dc.sampler.num_samples};
    auto const num_proj = 10u;
    auto const proj = Data::ProjectionStack<float, 2u>{proj_size, num_proj};
    REQUIRE(Recon::Detail::checkProjDetConsistency(cg, proj, true));
}

TEST_CASE_METHOD(Recon2DValidityTestFix,
    "Returns false since the 1D projection size and detector geometry are not consistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<1u>{dr.sampler.num_samples + 1u};
    auto const num_proj = 10u;
    auto const proj = Data::ProjectionStack<float, 1u>{proj_size, num_proj};
    REQUIRE_FALSE(Recon::Detail::checkProjDetConsistency(cg, proj, false));
    REQUIRE_FALSE(Recon::isValidAxialRecon(cg, proj, false));
}

TEST_CASE_METHOD(Recon3DValidityTestFix,
    "Returns false since the 2D projection size and detector geometry are not consistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<2u>{dr.sampler.num_samples,
                                              dc.sampler.num_samples + 1u};
    auto const num_proj = 10u;
    auto const proj = Data::ProjectionStack<float, 2u>{proj_size, num_proj};
    REQUIRE_FALSE(Recon::Detail::checkProjDetConsistency(cg, proj, false));
    REQUIRE_FALSE(Recon::isValidAxialRecon(cg, proj, false));
}

TEST_CASE_METHOD(Recon2DValidityTestFix,
    "Returns true since the number of 1D projections and geometry parameters are consistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<1u>{dr.sampler.num_samples};
    auto const num_proj = static_cast<unsigned >(pg.size());
    auto const proj = Data::ProjectionStack<float, 1u>{proj_size, num_proj};
    REQUIRE(Recon::Detail::checkProjViewConsistency(cg, proj, true));
}

TEST_CASE_METHOD(Recon3DValidityTestFix,
    "Returns true since the number of 2D projections and geometry parameters are consistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<2u>{dr.sampler.num_samples,
                                              dc.sampler.num_samples};
    auto const num_proj = static_cast<unsigned >(pg.size());
    auto const proj = Data::ProjectionStack<float, 2u>{proj_size, num_proj};
    REQUIRE(Recon::Detail::checkProjViewConsistency(cg, proj, true));
}

TEST_CASE_METHOD(Recon2DValidityTestFix,
    "Returns false since the number of 1D projections and geometry parameters are inconsistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<1u>{dr.sampler.num_samples};
    auto const num_proj = static_cast<unsigned>(pg.size() + 1u);
    auto const proj = Data::ProjectionStack<float, 1u>{proj_size, num_proj};
    REQUIRE_FALSE(Recon::Detail::checkProjViewConsistency(cg, proj, false));
    REQUIRE_FALSE(Recon::isValidAxialRecon(cg, proj, false));
}

TEST_CASE_METHOD(Recon3DValidityTestFix,
    "Returns false since the number of 2D projections and geometry parameters are inconsistent",
    "[geom][check_validity]")

{
    auto const proj_size = Data::ProjSize<2u>{dr.sampler.num_samples,
                                              dc.sampler.num_samples};
    auto const num_proj = static_cast<unsigned>(pg.size() - 1u);
    auto const proj = Data::ProjectionStack<float, 2u>{proj_size, num_proj};
    REQUIRE_FALSE(Recon::Detail::checkProjViewConsistency(cg, proj, false));
    REQUIRE_FALSE(Recon::isValidAxialRecon(cg, proj, false));
}

}//GCT::Test namespace.
