include "Dependencies.lua"

--Setup Things
workspace "DX12Sandbox"
configurations { "Debug", "Release", "Dist" }
	targetdir "build"
	startproject "DX12Sandbox"

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
include "DX12Sandbox/3rdParty/ImGui"
group ""


--Editor Premake lol
project "DX12Sandbox"
	location "DX12Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	
	files 
	{ 
		--App
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.c", 
		"%{prj.name}/Source/**.hpp", 
		"%{prj.name}/Source/**.cpp",

		--Engine
		"%{prj.name}/Engine/**.h", 
		"%{prj.name}/Engine/**.c", 
		"%{prj.name}/Engine/**.hpp", 
		"%{prj.name}/Engine/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/Source",
		"%{prj.name}/3rdParty",
		"%{prj.name}/Shaders",
		"%{prj.name}/Engine",
		"%{prj.name}/Textures",
		"%{IncludeDir.ImGui}",
	}

	links
	{
		"ImGui",
		--DX12 libs
		"d3d12.lib",
		"dxgi.lib",
		"dxcompiler.lib",
	}
	
	filter "system:windows"
		systemversion "latest"
				
		defines 
		{ 
			"APP_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		symbols "On"
		runtime "Debug"

		defines 
		{
			"RYAPP_DEBUG",
			"RYAPP_TRACK_MEMORY"
		}
				
	filter "configurations:Release"
		defines
		{
			"RYAPP_RELEASE",
			"RYAPP_TRACK_MEMORY"
		}

		runtime "Release"
		symbols "On"
		optimize "On"

	filter "configurations:Dist"
		defines "RYAPP_DIST"
		runtime "Release"
		optimize "On"
		symbols "Off"
