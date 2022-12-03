# CMakeList.txt : CMake project for Engine.Test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.24)

set (CMAKE_CXX_STANDARD 17)

enable_testing ()

find_package (GTest CONFIG REQUIRED)

file (GLOB_RECURSE TEST_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable (Test ${TEST_FILES})

target_compile_definitions (Test PRIVATE "ENGINE_PLATFORM_WINDOWS")

target_link_libraries (Test PRIVATE Engine GTest::gtest_main)

add_test (NAME Test COMMAND Test)
