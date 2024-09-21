project "SandboxApp"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	
	files 
	{ 
		--App
		"src/**.h", 
		"src/**.cpp" 
	}
	
	includedirs 
	{
		"src",
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