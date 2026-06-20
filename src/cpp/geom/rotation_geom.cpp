#include "rotation_geom.h"
#include <numeric>

namespace GCT::Geom
{

std::optional<RotDir> Detail::rotDir(std::vector<Units::Radians> const &rot_angles)
{
    auto rot_dir = RotDir{};

    if (rot_angles.size() > 1u) {
        auto const ang_first = Geom::Utils::wrapFrom0To2PI(rot_angles.at(0u));
        auto const ang_sec = Geom::Utils::wrapFrom0To2PI(rot_angles.at(1u));
        auto const quadr_first = Geom::Utils::quadrantIndex(ang_first);
        auto const quadr_sec = Geom::Utils::quadrantIndex(ang_sec);

        if ((quadr_first == 1u) && (quadr_sec == 4u)) {
            rot_dir = RotDir::CCW;
        }
        else if ((quadr_first == 4u) && (quadr_sec == 1u)) {
            rot_dir = RotDir::CW;
        }
        else {//Determine the rotation direction by computing the
            //difference between rotation angles.
            auto const diff = ang_sec - ang_first;
            if (diff
                > Units::Radians{0.0f}) {//Increasing rotation angle implies clockwise rotation.
                rot_dir = RotDir::CW;
            }
            else if (diff < Units::Radians{
                0.0f}) {//Decreasing rotation angle implies counter-clockwise rotation.
                rot_dir = RotDir::CCW;
            }
            else {
                return std::nullopt;//Ambiguous or stationary.
            }
        }
    }
    else {
        return std::nullopt;
    }
    return rot_dir;
}

std::optional<std::vector<Units::Radians>>
Detail::relRotAngles(std::vector<Units::Radians> const &rot_angles)
{
    auto const rot_dir = rotDir(rot_angles);
    if (rot_dir.has_value()) {

        auto rel_angles = std::vector<Units::Radians>{};
        rel_angles.reserve(rot_angles.size());

        auto const first_ang = rot_angles.front();
        std::transform(std::cbegin(rot_angles), std::cend(rot_angles),
                       std::back_inserter(rel_angles),
                       [first_ang, rot_dir](Units::Radians ang)
                       {
                           auto rel_ang = Units::Radians{0.0f};
                           if (rot_dir == RotDir::CCW) {
                               rel_ang = first_ang - ang;
                           }
                           else {
                               rel_ang = ang - first_ang;
                           }
                           return Utils::wrapFrom0To2PI(rel_ang);
                       }
        );
        return rel_angles;
    }
    else {
        return std::nullopt;
    }
}

std::optional<Units::Radians> Detail::rotAngIncr(std::vector<Units::Radians> const &rel_rot_ang)
{
    if (rel_rot_ang.empty()) {
        return std::nullopt;
    }

    auto ang_incr = std::vector<Units::Radians>{};
    ang_incr.reserve(rel_rot_ang.size());
    for (auto idx{0u}; idx < (rel_rot_ang.size() - 1u); idx++) {
        auto const incr = rel_rot_ang[idx + 1u] - rel_rot_ang[idx];
        ang_incr.push_back(Utils::wrapFrom0To2PI(incr));
    }
    return Units::Radians{std::accumulate(std::cbegin(ang_incr), std::cend(ang_incr),
                                          Units::Radians{0.0f}) /
        static_cast<float>(ang_incr.size())};
}

std::optional<Units::Radians> Detail::rotAngRange(std::vector<Units::Radians> const &rel_rot_ang)
{
    auto const rot_ang_icr = rotAngIncr(rel_rot_ang);
    if (rot_ang_icr.has_value()) {
        return rot_ang_icr.value() +
            (*std::max_element(std::cbegin(rel_rot_ang), std::cend(rel_rot_ang)));

    }
    else {
        return std::nullopt;
    }
}

std::optional<unsigned> Detail::numProjPerRot(std::vector<Units::Radians> const &rel_rot_ang)
{
    if (rel_rot_ang.empty()) {
        return std::nullopt;
    }
    auto constexpr first_ang = Units::Radians{0.0f};
    if (rel_rot_ang.front() != first_ang) {
        return std::nullopt;
    }
    auto rel_ang_clipped = rel_rot_ang;
    std::for_each(std::begin(rel_ang_clipped), std::end(rel_ang_clipped),
                  [](Units::Radians &ang)
                  {
                      if (std::fabs(ang.val) < 0.000001f) {
                          ang = Units::Radians{0.0f};
                      }
                  }
    );

    // Find whether the first angle is found later in the sequence.
    auto const first_ang_pos = std::find(std::cbegin(rel_ang_clipped) + 1u,
                                         std::cend(rel_ang_clipped), first_ang);
    if (first_ang_pos != std::cend(rel_ang_clipped)) {//The first angle is found again
        return static_cast<unsigned>(std::distance(std::cbegin(rel_ang_clipped), first_ang_pos));
    }
    else {//The first angle is not found again.
        return static_cast<unsigned>(rel_ang_clipped.size());
    }
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetParallel> const &pg)
{
    return Utils::wrapFrom0To2PI(pg.rot_ang);
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetFanArc> const &pg)
{
    return rotAng(pg.src_pos);
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetFanLine> const &pg)
{
    return rotAng(pg.src_pos);
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetParallel, DetParallel> const &pg)
{
    return Utils::wrapFrom0To2PI(pg.rot_ang);
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetParallel, DetFanLine> const &pg)
{
    return Utils::wrapFrom0To2PI(pg.rot_ang);
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetFanArc, DetFanLine> const &pg)
{
    return rotAng(pg.src_pos);
}

template<>
Units::Radians Detail::rotAng(ProjGeom<DetFanLine, DetFanLine> const &pg)
{
    return rotAng(pg.src_pos);
}

}//GCT::Geom namespace.


