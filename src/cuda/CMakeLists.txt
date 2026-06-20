project(gctcuda CUDA)
set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/eigen")
add_library(gctcuda
        numeric/comparator.cuh
        data/projection.cuh
        data/projection_stack.cuh
        recon/recon_wrappers.cuh
        recon/recon_wrappers.cu
        recon/fdk_axial.cuh
        recon/cos_weighting.cuh
        data/recon_img.cuh
        utils/cuda_config.cuh
        utils/cuda_config.cu
        recon/back_proj.cuh
        numeric/vec3d_math.cuh
        numeric/vec2d_math.cuh
        geom/coord_trans_proj.cuh
        geom/det_geom_dev.cuh
        recon/parker_weighting.cuh
        data/unified_mem.cuh
        data/nd_array_dev.cuh
        data/copy_host_dev.cuh
        data/dev_mem_texture.cuh
        data/stat.cuh
        recon/ramp_filtering_fft.cuh
        data/array_2d_dev.cuh
        recon/ramp_filtering_fft.cu
        data/dev_mem.cuh
        data/read_write_mem_resource.cuh
        data/host_mem_pinned.cuh
        utils/timer.cuh
        utils/timer.cu
        recon/forward_proj.cuh
        numeric/constants.cuh
        data/nd_array_texture.cuh
        geom/coord_trans_img.cuh
        geom/proj_geom_dev.cuh
        geom/ct_geom_dev.cuh
        utils/check_error.cuh
        utils/check_error.cu
        geom/ct_geom_dev.cu
        geom/vec_wcs_math.cuh numeric/arithm_functors.cuh recon/fdk_spiral.cuh
        recon/parker_weighting.cu)
target_link_libraries(gctcuda PRIVATE gctcpp)
target_link_libraries(gctcuda PRIVATE cufft)
set_target_properties(gctcuda PROPERTIES CUDA_SEPARABLE_COMPILATION OFF)
target_compile_options(gctcuda PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
        --expt-extended-lambda --expt-relaxed-constexpr -ftemplate-backtrace-limit=0 >)
target_include_directories(gctcuda PRIVATE ${EIGEN3_INCLUDE_DIR})