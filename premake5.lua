workspace "DreamTools"

	startproject "Sandbox"
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
	IncludeDir["GLAD"] = "DreamTools/vendor/GLAD/Include"
	IncludeDir["ImGui"] = "DreamTools/vendor/imgui"

	group "Dependencies"
		include "DreamTools/vendor/GLFW"
		include "DreamTools/vendor/GLAD"
		include "DreamTools/vendor/imgui"
	group ""

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		systemversion "latest"

	defines
	{
		"DT_PLATFORM_WINDOWS",
		"DT_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
	}

	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		runtime "Debug"
		symbols "On"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		runtime "Release"
		optimize "On"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		runtime "Release"
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
		staticruntime "off"
		systemversion "latest"

	defines
	{
		"DT_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		runtime "Debug"
		symbols "On"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		runtime "Release"
		optimize "On"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		runtime "Release"
		optimize "On"
	