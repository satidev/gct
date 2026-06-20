project(gctcpptest)

if (DCMTK_FOUND)
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")#For macOS and Homebrew is installed.
        link_directories("/opt/homebrew/opt/dcmtk/lib")
    endif()
    set(TEST_FILES_DCMTK_DEPENDENT io/file_reader/dcm_file_reader_tests.cpp)
else ()
    set(TEST_FILES_DCMTK_DEPENDENT)
endif ()

add_executable(gctcpptest
        main.cpp
        ../matchers.h
        ../matchers_test.cpp
        geom/utils_test.cpp
        numeric/comparator_test.cpp
        geom/proj_geom_factory_params_test.cpp
        geom/ct_geom_test.cpp
        data/nd_array_test.cpp
        data/projection_test.cpp
        data/projection_stack_test.cpp
        data/recon_img_test.cpp
        geom/proj_geom_params_test_fix.h
        geom/img_dim_test.cpp
        data/nd_array_test_fix.h
        data/stat_test.cpp
        io/projection_matrix_test.cpp
        geom/proj_geom_factory_proj_mat_test.cpp
        io/eigen_utils_test.cpp
        geom/vec_wcs_test.cpp
        geom/detector_1d_test.cpp
        geom/sampling_info_test.cpp
        geom/recon_img_geom_test.cpp
        geom/rotation_geom_test.cpp
        geom/proj_geom_test_fix.h
        geom/geom_math_test.cpp
        geom/ct_geom_test_fix.h
        geom/ct_geom_test_fix.cpp
        io/file_utils_test.cpp
        io/string_utils_test.cpp
        io/file_reader/array_file_reader_test_fix.cpp
        io/file_reader/array_file_reader_tests.cpp
        io/file_reader/mhd_file_reader_tests.cpp
        io/file_reader/raw_data_file_reader_tests.cpp
        io/file_reader/test_filenames.cpp
        io/file_writer/den_file_writer_tests.cpp
        io/file_writer/meta_img_file_writer_tests.cpp
        io/file_writer/raw_data_file_writer_tests.cpp
        ${TEST_FILES_DCMTK_DEPENDENT} data/proj_meta_info_test.cpp geom/units_test.cpp recon/check_validity_test.cpp geom/recon_img_geom_test_fix.h)

if (DCMTK_FOUND)
    target_link_libraries(gctcpptest PRIVATE ${DCMTK_LIBRARIES})
    target_include_directories(gctcpptest PRIVATE ${DCMTK_INCLUDE_DIRS})
endif ()

target_link_libraries(gctcpptest PRIVATE gctcpp)
target_include_directories(gctcpptest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/eigen)