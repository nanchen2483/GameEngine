# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)

set (CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE EDITOR_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable(Editor ${EDITOR_FILES})

target_include_directories (Editor PUBLIC "src")

target_link_libraries (Editor PRIVATE Engine)

target_compile_definitions (Editor
	PRIVATE
		"_CRT_SECURE_NO_WARNINGS"
		"ENGINE_PLATFORM_WINDOWS"
)

add_custom_target (EditorAssets ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_CURRENT_SOURCE_DIR}/assets
					${CMAKE_CURRENT_BINARY_DIR}/assets
					COMMENT "Copying assets into binary directory")

add_custom_target (EditorResources ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_CURRENT_SOURCE_DIR}/resources
					${CMAKE_CURRENT_BINARY_DIR}/resources
					COMMENT "Copying resources into binary directory")

add_dependencies (Editor EditorAssets EditorResources)

configure_file (imgui.ini imgui.ini COPYONLY)

if (MSVC)
	set_property (TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Editor)
endif()
