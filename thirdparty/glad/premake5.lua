project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	files {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs {
		"include"
	}

	filter "system:linux"
		pic "On"

		defines {
			"_GLAD_X11"
		}

	filter "system:windows"
		defines {
			"_GLAD_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
