project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/imd/" .. outputdir .. "/%{prj.name}")

	pchheader "enginepch.h"
	pchsource "src/enginepch.cpp"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.h",
		"%{IncludeDir.ImGuizmo}/ImGuizmo.cpp",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"assimp",
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib",
		"yaml-cpp"
	}

	filter "files:vendor/ImGuizmo/**.cpp" 
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "ENGINE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		runtime "Release"
		symbols "on"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		runtime "Release"
		symbols "on"
