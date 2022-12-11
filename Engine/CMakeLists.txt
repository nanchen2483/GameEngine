# CMakeList.txt : CMake project for Engine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.24)

set (CMAKE_CXX_STANDARD 17)

find_package (assimp CONFIG REQUIRED)
find_package (EnTT CONFIG REQUIRED)
find_package (glad CONFIG REQUIRED)
find_package (glfw3 CONFIG REQUIRED)
find_package (glm CONFIG REQUIRED)
find_package (imgui CONFIG REQUIRED)
find_package (imguizmo CONFIG REQUIRED)
find_package (spdlog CONFIG REQUIRED)
find_package (yaml-cpp CONFIG REQUIRED)

find_path (STB_INCLUDE_DIRS "stb_image.h" REQUIRED)
find_path (ICONS_FONT_AWESOME_INCLUDE_DIRS "IconsFontAwesome6.h" PATHS external/IconFontCppHeaders REQUIRED)

file (GLOB_RECURSE ENGINE_FILES "src/Engine/*.h" "src/Engine/*.cpp")
file (GLOB_RECURSE PLATFORM_FILES "src/Platform/*.h" "src/Platform/*.cpp")
file (GLOB_RECURSE EXTERNAL_FILES "external/*.h" "external/*.cpp")

set (CROSS_PLATFORM_FILES
	"src/Engine.h"
	"src/enginepch.cpp"
	"src/enginepch.h"

	${ENGINE_FILES}
	${PLATFORM_FILES}
	${EXTERNAL_FILES}
)

add_library (Engine STATIC ${CROSS_PLATFORM_FILES})

target_precompile_headers (Engine PRIVATE "src/enginepch.h")

target_include_directories (Engine PUBLIC "src" "external")

target_compile_definitions (Engine
	PRIVATE
		"_CRT_SECURE_NO_WARNINGS"
		"ENGINE_PLATFORM_WINDOWS"
		"GLFW_INCLUDE_NONE"

	PUBLIC
		$<$<CONFIG:Debug>:ENGINE_DEBUG>
		$<$<CONFIG:Release>:ENGINE_RELEASE>

		$<$<CONFIG:RelWithDebInfo>:ENGINE_DEBUG>
		$<$<CONFIG:MinSizeRel>:ENGINE_RELEASE>
)

target_link_libraries (Engine
	PRIVATE
		assimp::assimp
		EnTT::EnTT
		glad::glad
		glfw
		yaml-cpp

	PUBLIC
		glm::glm
		imgui::imgui
		imguizmo::imguizmo
		spdlog::spdlog
		spdlog::spdlog_header_only
)
