set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE engine_source_files "src/Engine/*.cpp")
file(GLOB_RECURSE engine_header_files "src/Engine/*.h")

set(CROSS_PLATFORM_FILES
	"src/Engine.h"
	"src/enginepch.cpp"
	"src/enginepch.h"

	${engine_source_files}
	${engine_header_files}

    "vendor/stb_image/stb_image.h"
	"vendor/stb_image/stb_image.cpp"

    "vendor/ImGuizmo/ImGuizmo.h"
    "vendor/ImGuizmo/ImGuizmo.cpp"
)

file(GLOB_RECURSE platform_source_files "src/Platform/*.cpp")
file(GLOB_RECURSE platform_header_files "src/Platform/*.h")

set(PLATFORM_FILES ${platform_source_files} ${platform_header_files})

add_library(Engine STATIC
	${CROSS_PLATFORM_FILES}
	${PLATFORM_FILES}
)

target_precompile_headers(Engine PUBLIC "src/enginepch.h")

target_include_directories(Engine PRIVATE
	"vendor/glfw/include"
	"vendor/glad/include"
)

set(ASSIMP_INCLUDE_DIRS "vendor/assimp/include" "${CMAKE_SOURCE_DIR}/Engine/vendor/assimp/include")

target_include_directories(Engine PUBLIC
	"src"
	${ASSIMP_INCLUDE_DIRS}
	"vendor/entt/include"
	"vendor/glm"
	"vendor/imgui"
    "vendor/ImGuizmo"
	"vendor/spdlog/include"
	"vendor/stb_image"
	"vendor/yaml-cpp/include"
)

target_compile_definitions(Engine
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

target_link_libraries(Engine PRIVATE glfw glad)
target_link_libraries(Engine PUBLIC imgui assimp yaml-cpp)
