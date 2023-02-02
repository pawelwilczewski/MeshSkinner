project "glm"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	includedirs { "glm/" }

	files {
		"glm/glm/**"
	}

	filter "system:windows"
		systemversion "latest"

		defines  { 
			"_GLM_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}


	filter "system:linux"
		pic "On"
		systemversion "latest"

		defines {
			"_GLM_X11"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
