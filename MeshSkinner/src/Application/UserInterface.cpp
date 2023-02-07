#include "pch.h"
#include "UserInterface.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "../imgui/backends/imgui_impl_opengl3.cpp"
#include "../imgui/backends/imgui_impl_glfw.cpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

glm::ivec2 UserInterface::GetViewportSize()
{
    // TODO: fix/implement this (we'll need dynamic target buffer)
    return glm::ivec2(500, 500);
}

void UserInterface::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(Window::GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void UserInterface::FrameBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UserInterface::FrameEnd()
{
    ImGuiIO &io = ImGui::GetIO();
    auto bufferSize = Window::GetFramebufferSize();
    io.DisplaySize = { (float)bufferSize.x, (float)bufferSize.y };

    // disable and store SRGB state
    const bool srgbEnabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_FRAMEBUFFER_SRGB);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // reenable srgb if necessary
    if (srgbEnabled)
        glEnable(GL_FRAMEBUFFER_SRGB);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void UserInterface::Terminate()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
