project(gctcudatest CUDA)
add_executable(gctcudatest
        main.cpp
        ../matchers.h
        numeric/comparator_test.cu
        img/proj_img_test.cu
        img/recon_img_test.cu
        data/unified_mem_test.cu
        data/nd_array_dev_test.cu
        data/vector_dev_test.cu
        data/copy_host_dev_test.cu
        data/array_2d_dev_test.cu
        data/stat_test.cu
        data/host_mem_pinned_test.cu
        data/dev_mem_test.cu
        geom/coord_trans_proj_test.cu data/nd_array_dev_test_fix.cuh geom/coord_trans_img_test.cu geom/vec_wcs_math_test.cu recon/fdk_axial_tests.cu
        recon/parker_weighting_test.cu)
target_link_libraries(gctcudatest PRIVATE gctcuda)
target_link_libraries(gctcudatest PRIVATE cufft)
target_link_libraries(gctcudatest PRIVATE gctcpp)
set_target_properties(gctcudatest PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_compile_options(gctcudatest PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
        --expt-extended-lambda --expt-relaxed-constexpr -ftemplate-backtrace-limit=0>)