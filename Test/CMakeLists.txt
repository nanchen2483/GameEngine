# CMakeList.txt : CMake project for Engine.Test, include source and define
# project specific logic here.
#
# Minimum required CMake version
cmake_minimum_required (VERSION 3.24)

# Project name
project (Test)

# Set the C++ standard to C++17
set (CMAKE_CXX_STANDARD 17)

# Enable testing
enable_testing ()

# Find Google Test package
find_package (GTest CONFIG REQUIRED)

# Collect source files
file (GLOB_RECURSE TEST_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${TEST_FILES})

# Set compile definitions
target_compile_definitions (${PROJECT_NAME}
	PRIVATE
		$<$<PLATFORM_ID:Windows>:ENGINE_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Linux>:ENGINE_PLATFORM_LINUX>
		"ENGINE_DYNAMIC_LINK"
		"ENGINE_TEST_DLL"
)

# Link required libraries
target_link_libraries (${PROJECT_NAME} PRIVATE Engine GTest::gtest_main)

# Add test
add_test (NAME Test COMMAND Test)
