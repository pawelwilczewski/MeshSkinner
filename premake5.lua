workspace "MeshSkinner"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	startproject "MeshSkinner"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "thirdparty/glfw"
	include "thirdparty/glad"
	include "thirdparty/imgui"
	include "thirdparty/tinygltf"
	include "thirdparty/glm"

group ""
	project "MeshSkinner"
		location "MeshSkinner"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

		pchheader "pch.h"
		pchsource "MeshSkinner/src/pch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/src",
			"thirdparty/glad/include",
			"thirdparty/glfw/include",
			"thirdparty/imgui",
			"thirdparty/glm",
			"thirdparty/spdlog/include",
			"thirdparty/tinygltf"
		}

		links
		{
			"glfw",
			"glad",
			"imgui",
			"opengl32.lib",
			"tinygltf",
			"glm"
		}

		defines {
			"GLFW_INCLUDE_NONE"
		}

		filter "system:windows"
			systemversion "latest"

			defines {
				"PLATFORM_WINDOWS"
			}

		filter "configurations:Debug"
			defines "DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "RELEASE"
			runtime "Release"
			optimize "on"

		filter "configurations:Dist"
			defines "DIST"
			runtime "Release"
			optimize "on"
			symbols "off"
