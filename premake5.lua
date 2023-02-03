workspace "MeshSkinner"
	architecture "x64"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	systemversion "latest"

	startproject "MeshSkinner"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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

	include "thirdparty/glfw"
	include "thirdparty/glad"
	include "thirdparty/imgui"
	include "thirdparty/tinygltf"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"thirdparty/glad/include",
		"thirdparty/glfw/include",
		"thirdparty/imgui",
		"thirdparty/glm",
		"thirdparty/spdlog/include",
		"thirdparty/tinygltf",
	}

	links {
		"glad",
		"glfw",
		"imgui",
		"tinygltf"
	}

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		defines {
			"PLATFORM_WINDOWS"
		}

	filter "system:linux"
		defines {
			"PLATFORM_LINUX"
		}
