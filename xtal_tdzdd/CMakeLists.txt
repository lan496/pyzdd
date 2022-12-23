cmake_minimum_required(VERSION 3.16)

project(xtal_tdzdd CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch TdZdd
include(FetchContent)
FetchContent_Declare(
  tdzdd
  GIT_REPOSITORY https://github.com/kunisura/TdZdd.git
  GIT_TAG        0073b8d2dd4b053002d17f342eb26467ad6ac2e8
)
FetchContent_MakeAvailable(tdzdd)

add_library(xtal_tdzdd STATIC
            iterator.cpp)

# https://cliutils.gitlab.io/modern-cmake/chapters/packages/OpenMP.html
# https://iscinumpy.gitlab.io/post/omp-on-high-sierra/
find_package(OpenMP REQUIRED)
if(NOT TARGET OpenMP::OpenMP_CXX)
    add_library(OpenMP_TARGET INTERFACE)
    add_library(OpenMP::OpenMP_CXX ALIAS OpenMP_TARGET)
    target_compile_options(OpenMP_TARGET INTERFACE ${OpenMP_CXX_FLAGS})
    find_package(Threads REQUIRED)
    target_link_libraries(OpenMP_TARGET INTERFACE Threads::Threads)
    target_link_libraries(OpenMP_TARGET INTERFACE ${OpenMP_CXX_FLAGS})
endif()
target_link_libraries(xtal_tdzdd PUBLIC OpenMP::OpenMP_CXX)

target_compile_options(xtal_tdzdd PRIVATE
                       -O3 -Wall -Winit-self -Wlogical-op)
set_target_properties(xtal_tdzdd PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(xtal_tdzdd PRIVATE ./)
target_include_directories(xtal_tdzdd PRIVATE ${tdzdd_SOURCE_DIR}/include)
