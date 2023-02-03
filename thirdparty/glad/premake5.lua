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
