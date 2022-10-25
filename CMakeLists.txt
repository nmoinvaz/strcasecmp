cmake_minimum_required(VERSION 3.17)
cmake_policy(SET CMP0091 NEW)

include(FetchContent)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_ENABLE_TESTING OFF)

# Search for Google benchmark package
find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
    FetchContent_Declare(benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main)

    FetchContent_GetProperties(benchmark)
    if(NOT benchmark_POPULATED)
        FetchContent_Populate(benchmark)
        add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif()
endif()

project(strcasecmp_benchmark CXX)

add_executable(strcasecmp_benchmark)

target_sources(strcasecmp_benchmark PRIVATE strcasecmp_benchmark.cc)
target_include_directories(strcasecmp_benchmark PRIVATE ${benchmark_SOURCE_DIR}/include)
target_link_directories(strcasecmp_benchmark PRIVATE ${benchmark_BINARY_DIR}/src/Release)
target_link_libraries(strcasecmp_benchmark benchmark shlwapi)
