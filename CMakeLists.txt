cmake_minimum_required(VERSION 3.23)

project(gct)

if (CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type was unspecified, set to Release")
    set(CMAKE_BUILD_TYPE Release)
else ()
    message(STATUS "Build type specified as '${CMAKE_BUILD_TYPE}'")
endif ()

set(CMAKE_CXX_STANDARD 20)
#[=[
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wall \
-Wextra \
-Werror \
-Wpedantic \
-Wconversion \
-Wdeprecated-declarations \
-Wformat \
-Wfloat-equal \
-Wshadow \
-Wuninitialized \
-Wunused \
-Wmissing-declarations \
-Wmissing-include-dirs \
-Wcast-align \
-Wredundant-decls \
-Wlogical-op \
-Wnull-dereference \
-Wreturn-type \
-Wsign-conversion \
-Warith-conversion \
-Walloc-zero \
-Wclass-memaccess \
-Wdouble-promotion \
-Wduplicated-branches \
-Wnon-virtual-dtor \
-Wnull-dereference \
-Wold-style-cast \
-Wunreachable-code")
]=]
# Exclude third party files from compiler warnings.
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/eigen")

include_directories(SYSTEM "${THIRDPARTY_DIR}")

# FIX 2: Use the modern CUDAToolkit package instead of the deprecated FindCUDA module.
find_package(CUDAToolkit)

add_subdirectory(src/cpp)

if(CUDAToolkit_FOUND)
    if(CUDAToolkit_VERSION VERSION_LESS "13.0")
        message(FATAL_ERROR "CUDA 13.0 or higher is required. Found: ${CUDAToolkit_VERSION}")
    endif()
    if(NOT DEFINED ENV{CUDAARCHS})
        # CUDA 13.0+ dropped support for architectures below sm_75
        # (Maxwell 5.x, Pascal 6.x, Volta 7.0/7.2). Start from Turing (7.5).
        set(CMAKE_CUDA_ARCHITECTURES 75 80 86 89 90)
    endif()

    set(CMAKE_CUDA_STANDARD 20)
    enable_language(CUDA)
    add_subdirectory(src/cuda)

    add_executable(gct main.cpp example_recons.h example_recons.cpp)
    target_link_libraries(gct PRIVATE gctcpp)
    target_link_libraries(gct PRIVATE gctcuda)
    set_target_properties(gct PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_include_directories(gct PRIVATE ${EIGEN3_INCLUDE_DIR})
endif(CUDAToolkit_FOUND)

add_subdirectory(test/cpp)

if(CUDAToolkit_FOUND)
    add_subdirectory(test/cuda)
endif(CUDAToolkit_FOUND)