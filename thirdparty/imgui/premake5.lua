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
		"imgui_demo.cpp",
		"misc/cpp/imgui_stdlib.cpp",
		"misc/cpp/imgui_stdlib.h",
		"backends/imgui_impl_opengl3.cpp",
		"backends/imgui_impl_glfw.cpp"
	}

	defines {
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

    includedirs {
        "",
        "../glad/include",
        "../glfw/include"
    }
