# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.22)

set (CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE XEDITOR_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable(xEditor ${XEDITOR_FILES})

target_include_directories (xEditor PRIVATE "src")

target_link_libraries (xEditor PRIVATE Engine)

target_compile_definitions (xEditor
	PRIVATE
		"_CRT_SECURE_NO_WARNINGS"
		"ENGINE_PLATFORM_WINDOWS"
)

add_custom_target (xEditorAssets ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/assets
					${CMAKE_CURRENT_BINARY_DIR}/assets
					COMMENT "Copying assets into binary directory")

add_custom_target (xEditorResources ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
					${CMAKE_CURRENT_BINARY_DIR}/resources
					COMMENT "Copying resources into binary directory")

add_dependencies (xEditor xEditorAssets xEditorResources)

configure_file (imgui.ini imgui.ini COPYONLY)

if (MSVC)
	set_property (TARGET xEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/xEditor)
endif()
