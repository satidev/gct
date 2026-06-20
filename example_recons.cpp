#include "example_recons.h"
#include "src/cpp/data/projection_stack.h"
#include "src/cpp/geom/axial_ct_geom.h"
#include "src/cuda/recon/recon_wrappers.cuh"
#include "src/cpp/io/parameters/proj_geom_params.h"
#include "src/cpp/data/recon_img.h"
#include "src/cpp/recon/check_validity.h"
#include "src/cpp/io/geom_reader/proj_geom_factory.h"
#include "src/cpp/io/proj_reader/projection_reader.h"
#include "src/cpp/io/file_reader/den_file_reader.h"
#include "src/cpp/io/file_reader/meta_img_reader.h"
#include "src/cpp/io/img_writer/img_writer.h"
#include "src/cpp/io/file_utils.h"

using namespace GCT::Geom;
using namespace GCT::Data;
using namespace GCT::IO;
using namespace std::string_literals;
using namespace GCT::Geom::Units;

namespace GCT::Examples
{
void recon1DParallelFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and volume (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"1dpdct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{1u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // 1D parallel projection geometry parameters for ideal circular trajectory.
        auto params = Parallel1DProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetParallel>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetParallel>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetParallel{proj.projSize().num_cols, 0.8961f};
        auto const det_geom = std::make_tuple(det_row);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetParallel>{det_geom, proj_geom};

        // Volume geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler {256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        VecWCS<2u>{0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}


void reconArcDetFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and image (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"1dadct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{1u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // Fan-beam projection geometry parameters for ideal circular trajectory.
        auto params = FanBeamProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.src_det_dist = 1000.0_mm;
        params.src_iso_dist = 500.0_mm;
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetFanArc>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetFanArc>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetFanArc{proj.projSize().num_cols, 0.0017922f};
        auto const det_geom = std::make_tuple(det_row);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetFanArc>{det_geom, proj_geom};

        // Recon image geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        VecWCS<2u>{0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

void reconLineDetFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and image (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"1dldct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{1u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // Fan-beam projection geometry parameters for ideal circular trajectory.
        auto params = FanBeamProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.src_det_dist = 1536.0_mm;
        params.src_iso_dist = 1000.0_mm;
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetFanLine>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetFanLine>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetFanLine{proj.projSize().num_cols, 2.0f};
        auto const det_geom = std::make_tuple(det_row);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetFanLine>{det_geom, proj_geom};

        // Volume geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        VecWCS<2u>{0.0f, 0.0f}//mid-point
        };
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

void recon2DParallelFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and image (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"2dpdct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{2u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // 2D parallel projection geometry parameters for ideal circular trajectory.
        auto params = Parallel2DProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.det_col_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetParallel, DetParallel>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetParallel, DetParallel>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetParallel{proj.projSize().num_cols, 0.8961f};
        auto const det_col = DetParallel{proj.projSize().num_rows, 1.14f};
        auto const det_geom = std::make_tuple(det_row, det_col);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetParallel, DetParallel>{det_geom, proj_geom};

        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        EqDistSampler{5u, 1.0f},//z-dimension
                                                        VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

void reconFlatDetFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and volume (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"2dfdct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{2u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // Cone-beam projection geometry parameters for ideal circular trajectory.
        auto params = ConeBeamProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.det_col_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
        params.src_det_dist = 1536.0_mm;
        params.src_iso_dist = 1000.0_mm;
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetFanLine, DetFanLine>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetFanLine, DetFanLine>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetFanLine{proj.projSize().num_cols, 2.0f};
        auto const det_col = DetFanLine{proj.projSize().num_rows, 2.0f};
        auto const det_geom = std::make_tuple(det_row, det_col);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetFanLine, DetFanLine>{det_geom, proj_geom};

        // Volume geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        EqDistSampler{5u, 1.0f},//z-dimension
                                                        VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

void reconCylDetFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and volume (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"2dcdct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{2u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // Cone-beam projection geometry parameters for ideal circular trajectory.
        auto params = ConeBeamProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.det_col_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
        params.src_det_dist = 1000.0_mm;
        params.src_iso_dist = 500.0_mm;
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetFanArc, DetFanLine>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetFanArc, DetFanLine>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetFanArc{proj.projSize().num_cols, 0.0017922f};
        auto const det_col = DetFanLine{proj.projSize().num_rows, 1.14f};
        auto const det_geom = std::make_tuple(det_row, det_col);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetFanArc, DetFanLine>{det_geom, proj_geom};

        // Volume geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        EqDistSampler{5u, 1.0f},//z-dimension
                                                        VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

void reconRebinDetFullScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and volume (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"2drdct"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{2u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // Cone-beam projection geometry parameters for ideal circular trajectory.
        auto params = ConeBeamProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = Radians{static_cast<float>(1.0 * std::numbers::pi / 180.0)};
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.det_col_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
        params.src_det_dist = 1000.0_mm;
        params.src_iso_dist = 500.0_mm;
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetParallel, DetFanLine>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetParallel, DetFanLine>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetParallel{proj.projSize().num_cols, 0.8961f};
        auto const det_col = DetFanLine{proj.projSize().num_rows, 1.14f};
        auto const det_geom = std::make_tuple(det_row, det_col);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetParallel, DetFanLine>{det_geom, proj_geom};

        // Volume geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        EqDistSampler{5u, 1.0f},//z-dimension
                                                        VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

void reconFlatDetShortScan(std::string const &test_data_dir_name)
{
    try {
        // Set projection (input) and volume (output) filenames.
        auto const dir_name = std::filesystem::path{test_data_dir_name} /
            std::string{"recon"} / std::string{"2dfdct_short"};
        auto const proj_filename = dir_name / std::string{"proj.mha"};
        auto const vol_filename = dir_name / std::string{"vol.mha"};

        // Read projections from the file.
        auto constexpr proj_dim = size_t{2u};
        auto const proj = ProjectionReader<MetaImgFileReader>::read<float, proj_dim>(proj_filename);

        // Cone-beam projection geometry parameters for ideal circular trajectory.
        auto params = ConeBeamProjGeomParams{};
        params.rot.init_ang = 0.0_rad;
        params.rot.num_proj = 360;
        params.rot.ang_incr = 0.0105434_rad;
        params.det_row_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_cols) - 1.0f) / 2.0f};
        params.det_col_center = Units::DetPixels{(static_cast<float>(proj.projSize().num_rows) - 1.0f) / 2.0f};
        params.src_det_dist = 1536.0_mm;
        params.src_iso_dist = 1000.0_mm;
        params.rot.hel_pitch = 0.0f;
        params.wcs = GCT::Geom::WCS::DCM;

        // Create projection geometry from the parameters.
        auto proj_geom = std::vector<ProjGeom<DetFanLine, DetFanLine>>{};
        proj_geom.reserve(params.rot.num_proj);
        for (auto proj_idx = 0u; proj_idx < params.rot.num_proj; proj_idx++) {
            auto const pg = ProjGeomFactory<DetFanLine, DetFanLine>::create(params, proj_idx);
            proj_geom.push_back(pg);
        }

        // Create detector geometry.
        auto const det_row = DetFanLine{proj.projSize().num_cols, 2.0f};
        auto const det_col = DetFanLine{proj.projSize().num_rows, 2.0f};
        auto const det_geom = std::make_tuple(det_row, det_col);

        // Complete CT geometry (detector + projection geometries).
        auto const ct_geom = AxialCTGeom<DetFanLine, DetFanLine>{det_geom, proj_geom};

        // Volume geometry.
        auto constexpr recon_img_dim = proj_dim + 1u;
        auto constexpr vg = ReconImgGeom<recon_img_dim>{EqDistSampler{256u, 1.0f},//x-dimension
                                                        EqDistSampler{256u, 1.0f},//y-dimension
                                                        EqDistSampler{5u, 1.0f},//z-dimension
                                                        VecWCS<3u>{0.0f, 0.0f, 0.0f}};//mid-point
        // Initialize volume.
        auto vol = ReconImg<float, recon_img_dim>{vg};

        // Reconstruct volume if the input data is valid.
        if (Recon::isValidAxialRecon(ct_geom, proj, true)) {
            Recon::CUDA::reconstruct(ct_geom, proj, vol);
            write(vol, vol_filename);
        }
        else {
            std::cerr << "Reconstruction cannot be performed due to invalid data." << std::endl;
        }
    }
    catch (std::exception const &e) {
        std::cerr << "Exception is thrown:: " << e.what() << std::endl;
    }
}

}//GCT::Examples namespace.
