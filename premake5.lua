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

		pchheader "pch.h"
		pchsource "MeshSkinner/src/pch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp",
			"thirdparty/glm/glm/**.hpp",
			"thirdparty/glm/glm/**.inl",
			"thirdparty/stb/**.h",
			"thirdparty/stb/**.cpp",
			-- "thirdparty/tinygltf/tiny_gltf.h",
			-- "thirdparty/tinygltf/tiny_gltf.cc"
		}

		includedirs
		{
			"%{prj.name}/src",
			"thirdparty/glad/include",
			"thirdparty/glfw/include",
			"thirdparty/imgui",
			"thirdparty/glm",
			"thirdparty/spdlog/include",
			"thirdparty/stb",
			-- "thirdparty/tinygltf"
		}

		links
		{
			"glfw",
			"glad",
			"imgui",
			"opengl32.lib"
		}

		defines {
			"_CRT_SECURE_NO_WARNINGS",
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
