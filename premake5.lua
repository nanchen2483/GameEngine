workspace "GameEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directory relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"] = "Engine/util/glfw/include"
IncludeDir["glad"] = "Engine/util/glad/include"
IncludeDir["glm"] = "Engine/util/glm"
IncludeDir["imgui"] = "Engine/util/imgui"
IncludeDir["stb_image"] = "Engine/util/stb_image"

group "Dependencies"
	include "Engine/util/glfw"
	include "Engine/util/glad"
	include "Engine/util/imgui"
group ""

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("imd/" .. outputdir .. "/%{prj.name}")

	pchheader "enginepch.h"
	pchsource "Engine/src/enginepch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/util/stb_image/**",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/util/spdlog/include",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib"
	}

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

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	cppdialect "C++17"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("imd/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Engine/util/spdlog/include",
		"Engine/util/imgui",
		"Engine/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
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