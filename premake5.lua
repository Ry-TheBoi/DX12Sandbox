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


--Engine
project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files 
	{ 
		--Engine
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"src",

		"%{prj.name}/3rdParty",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",

		"%{IncludeDir.ResourceLib}"
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


--ResourceLib
project "ResourceLib"
	location "ResourceLib"
	kind "SharedItems"

	files 
	{ 
		--Resources
		"%{prj.name}/Shaders/**.hlsl", 
		"%{prj.name}/Textures/**.png",
		"%{prj.name}/Textures/**.jpg", 
	}
	

--App
project "SandboxApp"
	location "SandboxApp"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	
	files 
	{ 
		--App
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp" 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{wks.location}/Engine/3rdParty",
		"%{wks.location}/Engine/src",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ResourceLib}"
	}

	links
	{
		"Engine",
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
		kind "WindowedApp"


