# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
# Minimum required CMake version
cmake_minimum_required (VERSION 3.24)

# Project name and version
project (GameEngine VERSION 0.0 DESCRIPTION "GameEngine" LANGUAGES C CXX)

# Set the C++ standard to C++17
set (CMAKE_CXX_STANDARD 17)

# Set the output directories for libraries and executables
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Editor)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Editor)

# Include sub-projects.
add_subdirectory (Engine)
add_subdirectory (Editor)
add_subdirectory (Test)
