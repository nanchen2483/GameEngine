project "Engine.Test"
	kind "ConsoleApp"
	cppdialect "C++17"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/imd/" .. outputdir .. "/%{prj.name}")

	pchheader "enginetestpch.h"
	pchsource "src/enginetestpch.cpp"

	nuget {"Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.5"}

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"%{wks.location}/Engine/src",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.spdlog}",
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
