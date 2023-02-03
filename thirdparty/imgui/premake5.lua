project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime "on"

	files {
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
	}

	includedirs {
		"../imgui/",
		"../glad/include",
		"../glfw/include/"
	}

	defines  {
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	filter "system:linux"
		pic "on"

		defines {
			"_IMGUI_X11"
		}

	filter "system:windows"
		defines  { 
			"_IMGUI_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}
