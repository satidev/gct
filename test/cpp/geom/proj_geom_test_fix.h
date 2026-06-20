#ifndef PROJ_GEOM_TEST_FIX_H
#define PROJ_GEOM_TEST_FIX_H

#include "proj_geom_params_test_fix.h"
#include "../../src/cpp/geom/detector_1d.h"
#include "../../src/cpp/geom/proj_geom.h"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"
#include <vector>

namespace GCT::Test
{
template<typename DetRow, typename ...DetCol>
struct ProjGeomTestFix
{
public:
    ProjGeomTestFix();

    std::vector<Geom::ProjGeom<DetRow, DetCol...>> projGeom() const noexcept
    {
        return pg;
    }
    std::vector<float> rotAng() const noexcept
    {
        return rot_ang_rad;
    }
    std::vector<float> relRotAng() const noexcept
    {
        return rel_rot_ang_rad;
    }
    float cmpThreshold() const noexcept
    {
        return 0.000001f;
    }
private:
    std::vector<Geom::ProjGeom<DetRow, DetCol...>> pg;
    typename ProjGeomParamsTypeFactory<DetRow, DetCol...>::type fix{};
    std::vector<float> rot_ang_rad;
    std::vector<float> rel_rot_ang_rad;
};

template<typename DetRow, typename... DetCol>
ProjGeomTestFix<DetRow, DetCol...>::ProjGeomTestFix()
{
    auto const params = fix.getParams();
    pg.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        pg.emplace_back(Geom::ProjGeomFactory<DetRow, DetCol...>::create(params, proj_idx));
    }

    rot_ang_rad.reserve(params.rot.num_proj);
    rel_rot_ang_rad.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const rel_rot_ang = static_cast<float>(proj_idx) * params.rot.ang_incr.val;
        rel_rot_ang_rad.push_back(rel_rot_ang);
        auto const rot_ang = Geom::Utils::wrapAngle(params.rot.init_ang.val + rel_rot_ang);
        rot_ang_rad.push_back(rot_ang);
    }
}

}//GCT::Test namespace.

#endif //PROJ_GEOM_TEST_FIX_H
