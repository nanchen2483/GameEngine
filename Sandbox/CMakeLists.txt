# CMakeList.txt : CMake project for Sandbox, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.22)

set (CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE SANDBOX_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable (Sandbox ${SANDBOX_FILES})

target_include_directories (Sandbox PRIVATE "src")

target_link_libraries (Sandbox PRIVATE Engine)

target_compile_definitions (Sandbox
	PRIVATE
		"_CRT_SECURE_NO_WARNINGS"
		"ENGINE_PLATFORM_WINDOWS"
)

add_custom_target (SandboxAssets ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/asserts
					${CMAKE_CURRENT_BINARY_DIR}/asserts
					COMMENT "Copying assets into binary directory")

add_custom_target (SandboxResources ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
					${CMAKE_CURRENT_BINARY_DIR}/resources
					COMMENT "Copying resources into binary directory")

add_dependencies (Sandbox SandboxAssets SandboxResources)

configure_file (imgui.ini imgui.ini COPYONLY)

if (MSVC)
	set_property (TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Sandbox)
endif()
