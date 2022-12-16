cmake_minimum_required(VERSION 3.4...3.18)
project(pyamtcpp)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG v2.10.1
)

FetchContent_GetProperties(pybind11)

if(NOT pybind11_POPULATED)
    FetchContent_Populate(pybind11)
    add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()

set(CMAKE_CXX_STANDARD 17)
pybind11_add_module(pyamtcpp src/main.cpp)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(pyamtcpp
    PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
