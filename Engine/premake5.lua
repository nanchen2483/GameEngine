project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/imd/" .. outputdir .. "/%{prj.name}")

	pchheader "enginepch.h"
	pchsource "src/enginepch.cpp"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"util/stb_image/**.h",
		"util/stb_image/**.cpp",
		"util/ImGuizmo/ImGuizmo.h",
		"util/ImGuizmo/ImGuizmo.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"util/spdlog/include",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib",
		"yaml-cpp"
	}

	filter "files:util/ImGuizmo/**.cpp" 
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
