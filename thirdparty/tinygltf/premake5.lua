project "tinygltf"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
    staticruntime "on"

	files {
		"tiny_gltf.h",
		"tiny_gltf.cc",
		"stb_image.h",
		"stb_image_write.h",
		"json.hpp"
	}
