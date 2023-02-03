project "tinygltf"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
    staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"tiny_gltf.h",
		"tiny_gltf.cc",
		"stb_image.h",
		"stb_image_write.h",
		"json.hpp"
	}
