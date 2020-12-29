workspace "DreamTools"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Include Directories relative to root (Solution Directory)
	IncludeDir = {}
	IncludeDir["GLFW"] = "DreamTools/vendor/GFLW/Include"
	include "DreamTools/vendor/GLFW"

project "DreamTools"
	location "DreamTools"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dtpch.h"
	pchsource "DreamTools/src/dtpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
 
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines
	{
		"DT_PLATFORM_WINDOWS",
		"DT_BUILD_DLL"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		symbols "On"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		optimize "On"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		optimize "On"
	

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
 
	includedirs
	{
		"DreamTools/vendor/spdlog/include",
		"DreamTools/src"
	}

	links
	{
		"DreamTools"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines
	{
		"DT_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		symbols "On"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		optimize "On"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		optimize "On"
	