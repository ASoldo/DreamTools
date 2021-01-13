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
	IncludeDir["glm"] = "DreamTools/vendor/glm"
	IncludeDir["stb_image"] = "DreamTools/vendor/stb_image"
	IncludeDir["entt"] = "DreamTools/vendor/entt/include"

	group "Dependencies"
		include "DreamTools/vendor/GLFW"
		include "DreamTools/vendor/GLAD"
		include "DreamTools/vendor/imgui"
	group ""

project "DreamTools"
	location "DreamTools"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dtpch.h"
	pchsource "DreamTools/src/dtpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
 
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		
		systemversion "latest"

	defines
	{
		"DT_PLATFORM_WINDOWS",
		"DT_BUILD_DLL",
		"GLFW_INCLUDE_NONE"
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		runtime "Debug"
		symbols "on"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		runtime "Release"
		optimize "on"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		runtime "Release"
		optimize "on"
	

	project "Sandbox"
		location "Sandbox"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

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
		"DreamTools/src",
		"DreamTools/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"DreamTools"
	}

	defines
	{
		"DT_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		runtime "Debug"
		symbols "on"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		runtime "Release"
		optimize "on"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		runtime "Release"
		optimize "on"


	project "Dreamer"
		location "Dreamer"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

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
		"DreamTools/src",
		"DreamTools/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"DreamTools"
	}

	defines
	{
		"DT_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "DT_DEBUG"
		runtime "Debug"
		symbols "on"
	

	filter "configurations:Release"
		defines "DT_RELEASE"
		runtime "Release"
		optimize "on"
	

	filter "configurations:Dist"
		defines "DT_DIST"
		runtime "Release"
		optimize "on"
