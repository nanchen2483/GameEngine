include "./util/premake/premake_customization/solution_items.lua"
include "dependencies.lua"

workspace "GameEngine"
	architecture "x64"
	startproject "xEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "util/premake"
	include "Engine/util/glfw"
	include "Engine/util/glad"
	include "Engine/util/imgui"
	include "Engine/util/yaml-cpp"
group ""

include "Engine"
include "Sandbox"
include "xEditor"
