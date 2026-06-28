#include "../../../thirdparty/catch.hpp"
#include "../../../src/cuda/recon/fdk_axial.cuh"
#include "../../test_data.h"
#include "../../../src/cpp/io/parameters/proj_geom_params.h"
#include "../../../src/cpp/io/file_reader/meta_img_reader.h"
#include "../../../src/cpp/io/geom_reader/proj_geom_factory.h"
#include <filesystem>
#include "../../matchers.h"

namespace GCT::Test
{
using namespace GCT::Geom::Units;


TEST_CASE("2D image is reconstructed from the 1D parallel projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"1dpdct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{1u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // 1D parallel projection geometry parameters for ideal circular trajectory.
    auto params = Geom::Parallel1DProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetParallel>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg = Geom::ProjGeomFactory<Geom::DetParallel>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetParallel{proj.projSize().num_cols, 0.8961f};
    auto const det_geom = std::make_tuple(det_row);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetParallel>{det_geom, proj_geom};

    // Volume geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::VecWCS<2u>{0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.000001f;
    Recon::CUDA::reconFDK<Geom::DetParallel>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetParallel>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetParallel>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("2D image is reconstructed from the 1D fan-beam (arc detector) projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"1dadct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{1u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // Fan-beam projection geometry parameters for ideal circular trajectory.
    auto params = Geom::FanBeamProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    using namespace Geom::Units;
    params.src_det_dist = 1000.0_mm;
    params.src_iso_dist = 500.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetFanArc>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg = Geom::ProjGeomFactory<Geom::DetFanArc>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetFanArc{proj.projSize().num_cols, 0.0017922f};
    auto const det_geom = std::make_tuple(det_row);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetFanArc>{det_geom, proj_geom};

    // Recon image geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::VecWCS<2u>{0.0f, 0.0f}};//mid-point                            

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.0001f;
    Recon::CUDA::reconFDK<Geom::DetFanArc>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetFanArc>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetFanArc>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("2D image is reconstructed from the 1D fan-beam (line detector) projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"1dldct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{1u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // Fan-beam projection geometry parameters for ideal circular trajectory.
    auto params = Geom::FanBeamProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    using namespace Geom::Units;
    params.src_det_dist = 1536.0_mm;
    params.src_iso_dist = 1000.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetFanLine>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg = Geom::ProjGeomFactory<Geom::DetFanLine>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetFanLine{proj.projSize().num_cols, 2.0f};
    auto const det_geom = std::make_tuple(det_row);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetFanLine>{det_geom, proj_geom};

    // Recon image geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::VecWCS<2u>{0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.000001f;
    Recon::CUDA::reconFDK<Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("3D volume is reconstructed from the 2D cone-beam (cylindrical detector) projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"2dcdct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{2u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // Cone-beam projection geometry parameters for ideal circular trajectory.
    auto params = Geom::ConeBeamProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    params.det_col_center = DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
    using namespace Geom::Units;
    params.src_det_dist = 1000.0_mm;
    params.src_iso_dist = 500.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetFanArc, Geom::DetFanLine>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const
            pg = Geom::ProjGeomFactory<Geom::DetFanArc, Geom::DetFanLine>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetFanArc{proj.projSize().num_cols, 0.0017922f};
    auto const det_col = Geom::DetFanLine{proj.projSize().num_rows, 1.14f};
    auto const det_geom = std::make_tuple(det_row, det_col);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetFanArc, Geom::DetFanLine>{det_geom, proj_geom};

    // Volume geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::EqDistSampler{5u, 1.0f},//z-dimension
        Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.0001f;
    Recon::CUDA::reconFDK<Geom::DetFanArc, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetFanArc, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetFanArc, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("3D volume is reconstructed from the 2D cone-beam (flat detector) projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"2dfdct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{2u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // Cone-beam projection geometry parameters for ideal circular trajectory.
    auto params = Geom::ConeBeamProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    params.det_col_center = DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
    using namespace Geom::Units;
    params.src_det_dist = 1536.0_mm;
    params.src_iso_dist = 1000.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetFanLine, Geom::DetFanLine>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg =
            Geom::ProjGeomFactory<Geom::DetFanLine, Geom::DetFanLine>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetFanLine{proj.projSize().num_cols, 2.0f};
    auto const det_col = Geom::DetFanLine{proj.projSize().num_rows, 2.0f};
    auto const det_geom = std::make_tuple(det_row, det_col);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetFanLine, Geom::DetFanLine>{det_geom, proj_geom};

    // Volume geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::EqDistSampler{5u, 1.0f},//z-dimension
        Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.000001f;
    Recon::CUDA::reconFDK<Geom::DetFanLine, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetFanLine, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetFanLine, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("3D volume is reconstructed from the 2D cone-beam (flat detector) short-scan projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"2dfdct_short"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{2u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // Cone-beam projection geometry parameters for ideal circular trajectory.
    auto params = Geom::ConeBeamProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{0.0105434f};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    params.det_col_center = DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
    using namespace Geom::Units;
    params.src_det_dist = 1536.0_mm;
    params.src_iso_dist = 1000.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetFanLine, Geom::DetFanLine>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg =
            Geom::ProjGeomFactory<Geom::DetFanLine, Geom::DetFanLine>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetFanLine{proj.projSize().num_cols, 2.0f};
    auto const det_col = Geom::DetFanLine{proj.projSize().num_rows, 2.0f};
    auto const det_geom = std::make_tuple(det_row, det_col);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetFanLine, Geom::DetFanLine>{det_geom, proj_geom};

    // Volume geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::EqDistSampler{5u, 1.0f},//z-dimension
        Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.000001f;
    Recon::CUDA::reconFDK<Geom::DetFanLine, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetFanLine, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetFanLine, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("3D volume is reconstructed from the 2D parallel projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"2dpdct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{2u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // 2D parallel projection geometry parameters for ideal circular trajectory.
    auto params = Geom::Parallel2DProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    params.det_col_center = DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetParallel, Geom::DetParallel>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg = Geom::ProjGeomFactory<Geom::DetParallel, Geom::DetParallel>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetParallel{proj.projSize().num_cols, 0.8961f};
    auto const det_col = Geom::DetParallel{proj.projSize().num_rows, 1.14f};
    auto const det_geom = std::make_tuple(det_row, det_col);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetParallel, Geom::DetParallel>{det_geom, proj_geom};

    // Volume geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::EqDistSampler{5u, 1.0f},//z-dimension
        Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.000001f;
    Recon::CUDA::reconFDK<Geom::DetParallel, Geom::DetParallel>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetParallel, Geom::DetParallel>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetParallel, Geom::DetParallel>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}

TEST_CASE("3D volume is reconstructed from the 2D cone-parallel projections",
          "[recon][fdk_axial]")
{
    // Set projection (input) and image (output) filenames.
    auto const dir_name = std::filesystem::path{TEST_FILE_DIR} /
        std::string{"recon"} / std::string{"2drdct"};
    auto const proj_filename = dir_name / std::string{"proj.mha"};

    // Read projections from the file.
    auto constexpr proj_dim = size_t{2u};
    auto const proj = IO::ProjectionReader<IO::MetaImgFileReader>::read<float, proj_dim>(
        proj_filename);

    // Cone-beam projection geometry parameters for ideal circular trajectory.
    auto params = Geom::ConeBeamProjGeomParams{};
    params.rot.init_ang = 0.0_rad;
    params.rot.num_proj = 360;
    params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
    params.det_row_center = DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
    params.det_col_center = DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
    using namespace Geom::Units;
    params.src_det_dist = 1000.0_mm;
    params.src_iso_dist = 500.0_mm;
    params.rot.hel_pitch = 0.0f;
    params.wcs = GCT::Geom::WCS::DCM;

    // Create projection geometry from the parameters.
    auto proj_geom = std::vector<Geom::ProjGeom<Geom::DetParallel, Geom::DetFanLine>>{};
    proj_geom.reserve(params.rot.num_proj);
    for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
        auto const pg = Geom::ProjGeomFactory<Geom::DetParallel, Geom::DetFanLine>::create(params, proj_idx);
        proj_geom.push_back(pg);
    }

    // Create detector geometry.
    auto const det_row = Geom::DetParallel{proj.projSize().num_cols, 0.8961f};
    auto const det_col = Geom::DetFanLine{proj.projSize().num_rows, 1.14f};
    auto const det_geom = std::make_tuple(det_row, det_col);

    // Complete CT geometry (detector + projection geometries).
    auto const ct_geom = Geom::AxialCTGeom<Geom::DetParallel, Geom::DetFanLine>{det_geom, proj_geom};

    // Volume geometry.
    auto constexpr recon_img_dim = proj_dim + 1u;
    auto constexpr vg = Geom::ReconImgGeom<recon_img_dim>{
        Geom::EqDistSampler{256u, 1.0f},//x-dimension
        Geom::EqDistSampler{256u, 1.0f},//y-dimension
        Geom::EqDistSampler{5u, 1.0f},//z-dimension
        Geom::VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point

    // Initialize volume.
    auto vol = Data::ReconImg<float, recon_img_dim>{vg};

    // Read the previously reconstructed volume from the file.
    auto const vol_filename = dir_name / std::string{"vol_ref.mha"};
    auto const reader = IO::MetaImgFileReader{vol_filename};
    auto const voxel_data = reader.template readData<float>();

    auto constexpr eps = 0.000001f;
    Recon::CUDA::reconFDK<Geom::DetParallel, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKLowMem<Geom::DetParallel, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
    Recon::CUDA::Detail::reconFDKHighMem<Geom::DetParallel, Geom::DetFanLine>(ct_geom, proj, vol);
    REQUIRE_THAT(vol.elements(), AreEqualFloatArrays(voxel_data, eps));
}


}//GCT::Test namespace.
