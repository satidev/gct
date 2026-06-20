#ifndef ROTATION_GEOM_H
#define ROTATION_GEOM_H

#include <vector>
#include <optional>
#include "proj_geom.h"

namespace GCT::Geom
{
enum class RotDir
{
    CCW = -1,// Counterclockwise rotation of gantry or source-detector pair.
    CW = 1 // Clockwise rotation of gantry.
};

struct RotationGeom
{
    RotDir dir;
    Units::Radians rot_ang_incr;
    Units::Radians rot_ang_range;
    unsigned num_proj_per_rot;
    std::vector<Units::Radians> rot_ang;
    std::vector<Units::Radians> rel_rot_ang;
};

template<typename DetRow, typename ...DetCol>
std::optional<RotationGeom> getRotationGeom(std::vector<ProjGeom<DetRow, DetCol...>> const &pg);

// Extract the rotation angle from the projection geometry parameters.
namespace Detail
{
template<size_t Dim>
constexpr Units::Radians rotAng(VecWCS<Dim> const &vec)
{
    return Utils::wrapFrom0To2PI(Units::Radians{std::atan2(vec.y.val, vec.x.val)});
}

template<typename DetRow, typename ...DetCol>
Units::Radians rotAng(ProjGeom<DetRow, DetCol...> const &pg);

// Rotation angles of the entire scan.
template<typename DetRow, typename ...DetCol>
std::vector<Units::Radians> rotAngles(std::vector<ProjGeom<DetRow, DetCol...>> const &fpg)
{
    if (fpg.empty()) {
        return std::vector<Units::Radians>{};
    }
    else {
        auto rot_ang = std::vector<Units::Radians>{};
        std::transform(std::cbegin(fpg), std::cend(fpg), std::back_inserter(rot_ang),
                       [](ProjGeom<DetRow, DetCol...> const &pg)
                       {
                           return rotAng(pg);
                       }
        );
        return rot_ang;
    }
}


// Computes the rotation direction of source-detector pair (gantry).
std::optional<RotDir> rotDir(std::vector<Units::Radians> const &rot_angles);

// Computes the rotation direction of source-detector pair (gantry).
template<typename DetRow, typename ...DetCol>
std::optional<RotDir> rotDir(std::vector<ProjGeom<DetRow, DetCol...>> const &fpg)
{
    auto const rot_angles = rotAngles(fpg);
    return rotDir(rot_angles);
}

// Relative rotation angles of the entire scan.
// Each rotation angle is subtracted by the rotation angle of the first projection.
// The first element of a relative rotation angle will always be zero.
// The output values will always be non-negative for both clockwise
// and counter-clockwise rotations.
std::optional<std::vector<Units::Radians>> relRotAngles(std::vector<Units::Radians> const &rot_angles);

// Rotation angular increment from relative rotation angles.
std::optional<Units::Radians> rotAngIncr(std::vector<Units::Radians> const &rel_rot_ang);

// Rotation angular range from relative rotation angles.
std::optional<Units::Radians> rotAngRange(std::vector<Units::Radians> const &rel_rot_ang);

// Number of projections per complete 360-degree rotation.
// Returns the number of angles if the angular range is less than 360 (e.g., short-scan)
std::optional<unsigned> numProjPerRot(std::vector<Units::Radians> const &rel_rot_ang);
}//Detail namespace.

template<typename DetRow, typename ...DetCol>
std::optional<RotationGeom> getRotationGeom(std::vector<ProjGeom<DetRow, DetCol...>> const &pg)
{
    auto info = RotationGeom{};
    info.rot_ang = Detail::rotAngles(pg);
    auto rot_dir = Detail::rotDir(info.rot_ang);
    if(rot_dir.has_value()){
        info.dir = rot_dir.value();
    }
    else{
        return std::nullopt;
    }

    auto rel_rot_ang = Detail::relRotAngles(info.rot_ang);
    if(rel_rot_ang.has_value()){
        info.rel_rot_ang = std::move(rel_rot_ang.value());
    }
    else{
        return std::nullopt;
    }
    info.rot_ang_incr = Detail::rotAngIncr(info.rel_rot_ang).value();
    info.rot_ang_range = Detail::rotAngRange(info.rel_rot_ang).value();
    info.num_proj_per_rot = Detail::numProjPerRot(info.rel_rot_ang).value();

    return info;
}

}//GCT::Geom namespace.

#endif //ROTATION_GEOM_H
