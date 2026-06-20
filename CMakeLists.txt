cmake_minimum_required(VERSION 3.23)
project(gct)

if (CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Build type was unspecified, set to Release")
    set(CMAKE_BUILD_TYPE Release)
else ()
    message(STATUS "Build type specified as '${CMAKE_BUILD_TYPE}'")
endif ()

set(CMAKE_CXX_STANDARD 20)

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



# Exclude third party files from compiler warnings.
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(EIGEN3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/eigen")
set(CMAKE_CXX_FLAGS -isystem"${THIRDPARTY_DIR}")


find_package(CUDA)
add_subdirectory(src/cpp)

if(CUDA_FOUND)
    if(NOT DEFINED $ENV{CUDAARCHS})
        set(CMAKE_CUDA_ARCHITECTURES 60 61 62 70 72 75 86)
    endif()
    set(CMAKE_CUDA_STANDARD 20)
    enable_language(CUDA)
    add_subdirectory(src/cuda)
    add_executable(gct main.cpp example_recons.h example_recons.cpp)
    target_link_libraries(gct PRIVATE gctcpp)
    target_link_libraries(gct PRIVATE gctcuda)
    set_target_properties(gct PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
    target_include_directories(gct PRIVATE ${EIGEN3_INCLUDE_DIR})
endif(CUDA_FOUND)

add_subdirectory(test/cpp)
if(CUDA_FOUND)
    add_subdirectory(test/cuda)
endif(CUDA_FOUND)













