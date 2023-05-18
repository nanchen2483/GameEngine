# CMakeList.txt : CMake project for Engine.Test, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.24)

project (Test)

set (CMAKE_CXX_STANDARD 17)

enable_testing ()

find_package (GTest CONFIG REQUIRED)

file (GLOB_RECURSE TEST_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${TEST_FILES})

target_compile_definitions (${PROJECT_NAME}
	PRIVATE
		$<$<PLATFORM_ID:Windows>:ENGINE_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Linux>:ENGINE_PLATFORM_LINUX>
		"ENGINE_DYNAMIC_LINK"
		"ENGINE_TEST_DLL"
)

target_link_libraries (${PROJECT_NAME} PRIVATE Engine GTest::gtest_main)

add_test (NAME Test COMMAND Test)
