#ifndef SPIRAL_CT_GEOM_H
#define SPIRAL_CT_GEOM_H

#include "ct_geom.h"

namespace GCT::Geom
{
// Currently, spiral scan geometry is available only for 2D detectors.
template<Valid1DDet DetRow, Valid1DDet DetCol>
struct SpiralCTGeom: public CTGeom<DetRow, DetCol>
{
public:
    SpiralCTGeom(std::tuple<DetRow, DetCol> const &t_dg,
                 std::vector<ProjGeom<DetRow, DetCol>> const &t_fpg);

    // First and last projection indices associated with slice position.
    // [first_idx, last_idx)
    // Spiral segment covering 2*pi angular range.
    std::pair<unsigned, unsigned> firstAndLastProjIndices(float slice_pos_mm) const noexcept;

private:
    float computeSpiralPitch(unsigned num_proj_per_rot);
    void setSrcPosZ();
    // Compute the projection location/index of given slice position.
    unsigned sliceProjIndex(float slice_pos_mm) const noexcept;

    std::vector<float> src_pos_z;
};

template<Valid1DDet DetRow, Valid1DDet DetCol>
SpiralCTGeom<DetRow, DetCol>::SpiralCTGeom(std::tuple<DetRow, DetCol> const &t_dg,
                                           std::vector<ProjGeom<DetRow, DetCol>> const &t_fpg)
    :
    CTGeom<DetRow, DetCol>{t_dg, t_fpg}
{
    if (this->rg.num_proj_per_rot == 0) {
        throw std::runtime_error{
            "Invalid spiral CT geometry: number of projections per rotation should not be zero.\n"};
    }
    setSrcPosZ();
    if ((!std::is_sorted(std::begin(src_pos_z), std::end(src_pos_z))) &&
        (!std::is_sorted(std::begin(src_pos_z), std::end(src_pos_z), std::greater<>()))) {
        throw std::runtime_error{
            "Invalid spiral CT geometry: the z-coordinate of source position should be sorted either in ascending  or descending order.\n"};
    }

    auto const spiral_pitch_mm = computeSpiralPitch(this->rg.num_proj_per_rot);
    if (spiral_pitch_mm <= 0.0f) {
        throw std::runtime_error{
            "Invalid spiral CT geometry: spiral pitch cannot be zero or negative.\n"};
    }
}

template<Valid1DDet DetRow, Valid1DDet DetCol>
unsigned SpiralCTGeom<DetRow, DetCol>::sliceProjIndex(float slice_pos_mm) const noexcept
{
    auto abs_diff = std::vector<float>{};
    std::transform(std::cbegin(src_pos_z), std::cend(src_pos_z), std::back_inserter(abs_diff),
                   [slice_pos_mm](float src_pos)
                   { return std::fabs(src_pos - slice_pos_mm); });

    auto const min_pos = std::min_element(std::begin(abs_diff), std::end(abs_diff));
    return static_cast<unsigned>(std::distance(std::begin(abs_diff), min_pos));
}

template<Valid1DDet DetRow, Valid1DDet DetCol>
std::pair<unsigned, unsigned>
SpiralCTGeom<DetRow, DetCol>::firstAndLastProjIndices(float slice_pos_mm) const noexcept
{
    auto const middle_proj_idx = sliceProjIndex(slice_pos_mm);
    auto const first_proj_idx =  middle_proj_idx - this->rg.num_proj_per_rot / 2u;
    auto const last_proj_idx = first_proj_idx + this->rg.num_proj_per_rot;
    return std::make_pair(first_proj_idx, last_proj_idx);
}

template<Valid1DDet DetRow, Valid1DDet DetCol>
void SpiralCTGeom<DetRow, DetCol>::setSrcPosZ()
{
    std::transform(std::cbegin(this->projGeom()), std::cend(this->projGeom()),
                   std::back_inserter(src_pos_z),
                   [](Geom::ProjGeom<DetRow, DetCol> const &pg)
                   {
                       return pg.src_pos.z;
                   }
    );
}

template<Valid1DDet DetRow, Valid1DDet DetCol>
float SpiralCTGeom<DetRow, DetCol>::computeSpiralPitch(unsigned int num_proj_per_rot)
{
    // Displacement of the X-ray source along z-axis per rotation.
    auto const src_z_disp_mm = std::fabs(this->projGeom().front().src_pos.z -
        this->projGeom().at(num_proj_per_rot).src_pos.z);
    return src_z_disp_mm / (2.0f * std::numbers::pi_v<float>);
}

}//GCT::Geom namespace.

#endif //SPIRAL_CT_GEOM_H
