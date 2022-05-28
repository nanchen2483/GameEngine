# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.22)
project ("GameEngine")
set(CMAKE_CXX_STANDARD 17)
project(GameEngine VERSION 0.0 DESCRIPTION "Game engine" LANGUAGES C CXX)

# Vendor
add_subdirectory(Engine/vendor/glfw)
add_subdirectory(Engine/vendor/glad)
add_subdirectory(Engine/vendor/imgui)
add_subdirectory(Engine/vendor/yaml-cpp)
add_subdirectory(Engine/vendor/assimp)

# Include sub-projects.
add_subdirectory(Engine)
add_subdirectory(xEditor)
add_subdirectory(Sandbox)
