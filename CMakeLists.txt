cmake_minimum_required(VERSION 3.19)
project(OpenCVFaceDetectorCPP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

add_library(project_options INTERFACE)
include(cmake/CompilerOptions.cmake)
set_project_options(project_options)

add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

include(cmake/StaticAnalyzers.cmake)

find_package(spdlog REQUIRED)
find_package(OpenCV REQUIRED)
include(CTest)
find_package(Catch2 REQUIRED)
include(Catch)
add_subdirectory(src)
