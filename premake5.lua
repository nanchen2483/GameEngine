include "./vendor/premake/premake_customization/solution_items.lua"
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
	include "Engine/vendor/assimp"
	include "Engine/vendor/glfw"
	include "Engine/vendor/glad"
	include "Engine/vendor/imgui"
	include "Engine/vendor/yaml-cpp"
group ""

include "Engine"
include "Sandbox"
include "xEditor"
include "Engine.Test"
