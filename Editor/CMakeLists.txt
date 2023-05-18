# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.24)

set (CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE EDITOR_FILES "src/*.h" "src/*.cpp")

# Add source to this project's executable.
add_executable(Editor ${EDITOR_FILES})

target_include_directories (Editor PUBLIC "src")

target_link_libraries (Editor PRIVATE Engine)

target_compile_definitions (Editor
	PRIVATE
		$<$<PLATFORM_ID:Windows>:ENGINE_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Linux>:ENGINE_PLATFORM_LINUX>
		"_CRT_SECURE_NO_WARNINGS"
		"ENGINE_DYNAMIC_LINK"
)

if (MSVC)
	# Hide the console window in Visual Studio projects - Release
	set_target_properties (Editor
		PROPERTIES
			LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
			LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

	# Sets the local debugger working directory for Visual Studio
	set_property (TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Editor)
endif()

# Copy assets, resources and imgui.ini to binary directory
add_custom_target (assets ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/assets
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
	COMMENT "Copying assets into binary directory")

add_custom_target (resources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_CURRENT_SOURCE_DIR}/resources
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
	COMMENT "Copying resources into binary directory")

add_dependencies (Editor assets resources)

file (CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/imgui.ini)
