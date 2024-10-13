project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "Ry-Engine_PCH.h"
	pchsource "src/Ry-Engine_PCH.cpp"
	
	files 
	{ 
		--Engine
		"src/**.h", 
		"src/**.cpp" 
	}
	
	includedirs 
	{
		"src",

		"3rdParty",
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
		"dxguid.lib",
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