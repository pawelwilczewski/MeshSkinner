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

group ""
	project "MeshSkinner"
		location "MeshSkinner"
		kind "StaticLib"
		language "C++"
		cppdialect "C++17"
		staticruntime "on"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

		pchheader "MeshSkinner/src/pch.h"
		pchsource "MeshSkinner/src/pch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"thirdparty/glm/glm/**.hpp",
			"thirdparty/glm/glm/**.inl",
			"thirdparty/stb_image/**.h",
			"thirdparty/stb_image/**.cpp",
			"thirdparty/tinygltf/**.h",
			"thirdparty/tinygltf/**.cpp"
		}

		includedirs
		{
			"%{prj.name}/src",
			"thirdparty/glad/include",
			"thirdparty/glfw/include",
			"thirdparty/imgui",
			"thirdparty/glm",
			"thirdparty/spdlog/include",
			"thirdparty/stb_image",
			"thirdparty/tinygltf"
		}

		links
		{
			"glfw",
			"glad",
			"imgui",
			"opengl32.lib"
		}

		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}

		filter "system:windows"
			systemversion "latest"

			defines {
				"PLATFORM_WINDOWS",
				"GLFW_INCLUDE_NONE",
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
