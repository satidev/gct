#include "../../../thirdparty/catch.hpp"
#include "ct_geom_test_fix.h"
#include "../../matchers.h"
#include "../../../src/cpp/geom/geom_math.h"

namespace GCT::Test
{
TEMPLATE_TEST_CASE_SIG("The geometry of 1D detector is initialized correctly",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetParallel, 0u),
                       (Geom::DetFanArc, 0u),
                       (Geom::DetFanLine, 0u))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &cg = fix.ctGeom();
    auto const &[dr] = cg.detGeom();

    REQUIRE(dr.sampler.num_samples == 1u);
    REQUIRE(dr.sampler.sampling_width.val == 1.0f);
}

TEMPLATE_TEST_CASE_SIG("The geometry of 2D detector is initialized correctly",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetParallel, 0u, Geom::DetParallel),
                       (Geom::DetParallel, 0u, Geom::DetFanLine),
                       (Geom::DetFanArc, 0u, Geom::DetFanLine),
                       (Geom::DetFanLine, 0u, Geom::DetFanLine))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &cg = fix.ctGeom();
    auto const &[dr, dc] = cg.detGeom();

    REQUIRE(dr.sampler.num_samples == 1u);
    REQUIRE(dr.sampler.sampling_width.val == 1.0f);
    REQUIRE(dc.sampler.num_samples == 2u);
    REQUIRE(dc.sampler.sampling_width.val == 2.0f);
}

TEMPLATE_TEST_CASE_SIG("Source-detector distance is initialized correctly",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetFanArc, 0u),
                       (Geom::DetFanLine, 0u),
                       (Geom::DetParallel, 0u, Geom::DetFanLine),
                       (Geom::DetFanArc, 0u, Geom::DetFanLine),
                       (Geom::DetFanLine, 0u, Geom::DetFanLine))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &pg = fix.ctGeom().projGeom();
    auto const params = typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type{};
    auto const src_det_dist = params.getParams().src_det_dist;
    std::for_each(std::cbegin(pg), std::cend(pg),
                  [src_det_dist](auto const &pg_view)
                  {
                      REQUIRE(pg_view.src_det_dist == src_det_dist);
                  }
    );
}

TEMPLATE_TEST_CASE_SIG("Source-iso-center distance is correctly computed from the source position",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetFanArc, 0u),
                       (Geom::DetFanLine, 0u),
                       (Geom::DetFanArc, 0u, Geom::DetFanLine),
                       (Geom::DetFanLine, 0u, Geom::DetFanLine))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &pg = fix.ctGeom().projGeom();
    auto const params = typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type{};
    auto const src_iso_dist = params.getParams().src_iso_dist;

    std::for_each(std::cbegin(pg), std::cend(pg),
                  [src_iso_dist](auto const &pg_view)
                  {
                      REQUIRE(Geom::Math::norm(pg_view.src_pos) == src_iso_dist.val);
                  }
    );
}

TEMPLATE_TEST_CASE_SIG("The center of detector row is initialized correctly",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetParallel, 0u),
                       (Geom::DetFanArc, 0u),
                       (Geom::DetFanLine, 0u),
                       (Geom::DetParallel, 0u, Geom::DetParallel),
                       (Geom::DetParallel, 0u, Geom::DetFanLine),
                       (Geom::DetFanArc, 0u, Geom::DetFanLine),
                       (Geom::DetFanLine, 0u, Geom::DetFanLine))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &pg = fix.ctGeom().projGeom();
    auto const params = typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type{};
    auto const det_row_cent = params.getParams().det_row_center;

    std::for_each(std::cbegin(pg), std::cend(pg),
                  [det_row_cent](auto const &pg_view)
                  {
                      REQUIRE(pg_view.det_row_center == det_row_cent);
                  }
    );
}

TEMPLATE_TEST_CASE_SIG("The center of detector column is initialized correctly",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetParallel, 0u, Geom::DetParallel),
                       (Geom::DetParallel, 0u, Geom::DetFanLine),
                       (Geom::DetFanArc, 0u, Geom::DetFanLine),
                       (Geom::DetFanLine, 0u, Geom::DetFanLine))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &pg = fix.ctGeom().projGeom();
    auto const params = typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type{};
    auto const det_col_cent = params.getParams().det_col_center;

    std::for_each(std::cbegin(pg), std::cend(pg),
                  [det_col_cent](auto const &pg_view)
                  {
                      REQUIRE(pg_view.det_col_center == det_col_cent);
                  }
    );
}

TEMPLATE_TEST_CASE_SIG("Rotation angle is computed correctly",
                       "[geom][ct_geom]",
                       ((typename DetRow, size_t Dummy, typename...DetCol), DetRow, Dummy, DetCol...),
                       (Geom::DetParallel, 0u),
                       (Geom::DetParallel, 0u, Geom::DetParallel),
                       (Geom::DetParallel, 0u, Geom::DetFanLine))

{
    using namespace Geom;
    auto const fix = CTGeomTestFix<DetRow, DetCol...>{};
    auto const &pg = fix.ctGeom().projGeom();
    auto const params = typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type{};

    auto const rot_ang = ProjGeomTestFix<DetRow, DetCol...>{}.rotAng();

    for (auto proj_idx = 0; proj_idx < params.getParams().rot.num_proj; proj_idx++) {
        REQUIRE(Geom::Utils::wrapAngle(pg.at(proj_idx).rot_ang.val) ==
            Geom::Utils::wrapAngle(rot_ang.at(proj_idx)));
    }
}

}//GCT::Test namespace.


