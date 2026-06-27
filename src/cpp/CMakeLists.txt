project(gctcpp)

set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/eigen")

find_package(DCMTK)

if (DCMTK_FOUND)
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")#For macOS and Homebrew is installed.
        link_directories("/opt/homebrew/opt/dcmtk/lib")
    endif()
    set(SRC_FILES_DCMTK_DEPENDENT
            io/file_reader/dcm_file_reader.h
            io/file_reader/dcm_file_reader.cpp)
else()
    set(SRC_FILES_DCMTK_DEPENDENT)
endif ()

add_library(gctcpp STATIC
        geom/proj_geom.h
        geom/ct_geom.h
        geom/detector_1d.h
        geom/utils.h
        geom/vec_wcs.h
        numeric/comparator.h
        data/nd_array.h
        data/projection.h
        data/projection_stack.h
        geom/recon_img_geom.h
        data/recon_img.h
        recon/ramp_filter.h
        recon/proj_filt_kernel.h
        recon/proj_filt_kernel.cpp
        recon/ramp_filter.cpp
        data/proj_meta_info.h
        data/proj_meta_info.cpp
        recon/check_validity.h
        data/stat.h
        geom/axial_ct_geom.h
        geom/spiral_ct_geom.h
        geom/projection_matrix.h
        geom/projection_matrix.cpp
        geom/eigen_utils.h
        geom/eigen_utils.cpp
        geom/sampling_info.h
        io/parameters/proj_geom_params.h
        io/geom_reader/proj_geom_factory.h
        io/geom_reader/proj_geom_factory_params.cpp
        io/geom_reader/proj_geom_cb_factory.h
        io/geom_reader/proj_geom_cb_factory.cpp
        io/geom_reader/proj_geom_fb_factory.h
        io/geom_reader/proj_geom_fb_factory.cpp
        io/geom_reader/proj_geom_factory_proj_mat.cpp
        io/parameters/filt_parameters.h
        geom/geom_constraints.h
        data/data_constraints.h
        geom/geom_math.h
        geom/geom_math.cpp
        geom/rotation_geom.h
        geom/rotation_geom.cpp
        geom/vec_wcs.cpp
        io/enum_parser.h
        io/enum_parser.cpp
        io/file_utils.h
        io/file_utils.cpp
        io/string_utils.cpp
        io/file_reader/array_file_reader.h
        io/file_reader/array_file_reader.cpp
        io/file_reader/den_file_reader.h
        io/file_reader/den_file_reader.cpp
        io/file_reader/meta_img_reader.h
        io/file_reader/meta_img_reader.cpp
        io/file_reader/raw_data_file_reader.cpp
        io/file_writer/den_file_writer.h
        io/file_writer/meta_img_file_writer.cpp
        ${SRC_FILES_DCMTK_DEPENDENT}
        io/proj_reader/projection_reader.h
        io/proj_reader/projection_reader.cpp
        io/img_writer/img_writer.h
        io/img_writer/img_writer.cpp
        io/geom_reader/proj_mat_reader.h geom/units.h geom/det_coords.h geom/det_pixel_coords.h
        data/nd_array_base.h)

if (DCMTK_FOUND)
    target_link_libraries(gctcpp PRIVATE ${DCMTK_LIBRARIES})
    target_include_directories(gctcpp PRIVATE ${DCMTK_INCLUDE_DIRS})
endif ()

target_include_directories(gctcpp PRIVATE ${EIGEN3_INCLUDE_DIR})



