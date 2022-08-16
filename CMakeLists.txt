cmake_minimum_required(VERSION 3.16)

project(pyzdd CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch TdZdd and pybind11
include(FetchContent)
FetchContent_Declare(
  tdzdd
  GIT_REPOSITORY https://github.com/kunisura/TdZdd.git
  GIT_TAG        0073b8d2dd4b053002d17f342eb26467ad6ac2e8
)
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.10.0
)
FetchContent_MakeAvailable(tdzdd)
FetchContent_MakeAvailable(pybind11)

add_compile_options(-O3)
include_directories(${tdzdd_SOURCE_DIR}/include)

# Build `xtal_tdzdd` library
add_subdirectory(./xtal_tdzdd)

message(STATUS ${pybind11_SOURCE_DIR})
message(STATUS ${pybind11_BINARY_DIR})

add_subdirectory("${pybind11_SOURCE_DIR}" pybind11)
pybind11_add_module(pyzdd_ ./xtal_tdzdd/wrapper.cpp)
target_link_libraries(pyzdd_ PRIVATE xtal_tdzdd)
