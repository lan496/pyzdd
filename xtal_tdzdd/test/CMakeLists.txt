cmake_minimum_required(VERSION 3.16)

project(test_xtal_tdzdd CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Fetch TdZdd and GoogleTest
include(FetchContent)
FetchContent_Declare(
  tdzdd
  GIT_REPOSITORY https://github.com/kunisura/TdZdd.git
  GIT_TAG        0073b8d2dd4b053002d17f342eb26467ad6ac2e8
)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(tdzdd)
FetchContent_MakeAvailable(googletest)

# Include TdZdd
include_directories(${tdzdd_SOURCE_DIR}/include)
# include(CMakePrintHelpers)
# cmake_print_variables(tdzdd_SOURCE_DIR)

include_directories(../)

# OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_compile_options(-O3 -Wall -Winit-self -Wlogical-op -fsanitize=address)
add_link_options(-fsanitize=address)

enable_testing()
include(GoogleTest)
foreach(TESTCASE IN ITEMS
        test_choice test_combination test_simpath test_stpath test_spanning_forest
        test_permutation test_superperiodic test_structure_enumeration
        test_induced_graph test_sro)
  add_executable(${TESTCASE} ${TESTCASE}.cpp)
  target_link_libraries(${TESTCASE} gtest_main)
  gtest_discover_tests(${TESTCASE})
endforeach()
