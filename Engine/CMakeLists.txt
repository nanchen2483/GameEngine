# CMakeList.txt : CMake project for Engine, include source and define
# project specific logic here.
#
# Minimum required CMake version
cmake_minimum_required (VERSION 3.24)

# Project name
project (Engine)

# Set the C++ standard to C++17
set (CMAKE_CXX_STANDARD 17)

# Build shared libraries
set (BUILD_SHARED_LIBS TRUE)

# Find required packages
find_package (assimp CONFIG REQUIRED)
find_package (EnTT CONFIG REQUIRED)
find_package (glad CONFIG REQUIRED)
find_package (glfw3 CONFIG REQUIRED)
find_package (glm CONFIG REQUIRED)
find_package (imgui CONFIG REQUIRED)
find_package (imguizmo CONFIG REQUIRED)
find_package (spdlog CONFIG REQUIRED)
find_package (yaml-cpp CONFIG REQUIRED)

# Find Linux-specific packages
if (UNIX AND NOT APPLE)
	find_package (PkgConfig REQUIRED)
	pkg_check_modules (GTK3 REQUIRED gtk+-3.0)
endif()

# Find additional include directories
find_path (STB_INCLUDE_DIRS "stb_image.h" REQUIRED)
find_path (ICONS_FONT_AWESOME_INCLUDE_DIRS "IconsFontAwesome6.h" PATHS external/IconFontCppHeaders REQUIRED)

# Collect source files
file (GLOB_RECURSE ENGINE_FILES "src/Engine/*.h" "src/Engine/*.cpp")
file (GLOB_RECURSE PLATFORM_FILES "src/Platform/*.h" "src/Platform/*.cpp")
file (GLOB_RECURSE EXTERNAL_FILES "external/*.h" "external/*.cpp")

# Create a list of cross-platform files
set (CROSS_PLATFORM_FILES
	"src/Engine.h"
	"src/enginepch.cpp"
	"src/enginepch.h"

	${ENGINE_FILES}
	${PLATFORM_FILES}
	${EXTERNAL_FILES}
)

# Create the Engine library target
add_library (${PROJECT_NAME} SHARED ${CROSS_PLATFORM_FILES})

# Set precompiled headers
target_precompile_headers (${PROJECT_NAME} PRIVATE "src/enginepch.h")

# Set include directories
target_include_directories (${PROJECT_NAME} PUBLIC "src" "external")

# Set compile definitions
target_compile_definitions (${PROJECT_NAME}
	PRIVATE
		$<$<PLATFORM_ID:Windows>:ENGINE_PLATFORM_WINDOWS>
        $<$<PLATFORM_ID:Linux>:ENGINE_PLATFORM_LINUX>
		"_CRT_SECURE_NO_WARNINGS"
		"ENGINE_GRAPHICS_API_OPENGL"
		"ENGINE_DYNAMIC_LINK"
		"ENGINE_BUILD_DLL"
		"GLFW_INCLUDE_NONE"
	PUBLIC
		$<$<CONFIG:Debug>:ENGINE_DEBUG>
		$<$<CONFIG:Release>:ENGINE_RELEASE>
		$<$<CONFIG:RelWithDebInfo>:ENGINE_DEBUG>
		$<$<CONFIG:MinSizeRel>:ENGINE_RELEASE>
)

# Link required libraries
target_link_libraries (${PROJECT_NAME}
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

# Add Linux-specific configurations
if (UNIX AND NOT APPLE)
	target_include_directories (${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
	target_link_libraries (${PROJECT_NAME} PRIVATE ${GTK3_LIBRARIES})
endif ()
