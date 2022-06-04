# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.22)

project (GameEngine VERSION 0.0 DESCRIPTION "GameEngine" LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)

# Include sub-projects.
add_subdirectory (Engine)
add_subdirectory (Editor)
add_subdirectory (Sandbox)
add_subdirectory (Test)
