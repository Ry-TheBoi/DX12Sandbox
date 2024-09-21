include "Dependencies.lua"

--Setup Things
workspace "DX12Sandbox"
configurations { "Debug", "Release", "Dist" }
	targetdir "build"
	startproject "SandboxApp"

	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter "language:C++ or language:C"
		architecture "x86_64"
	filter ""
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

--External Dependencis Folder
group "Dependencies"
	include "Engine/3rdParty/ImGui"
group ""

	
include "Engine"
include "ResourceLib"
include "SandboxApp"