#include "pch.h"
#include "UserInterface.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include "../imgui/backends/imgui_impl_opengl3.cpp"
#include "../imgui/backends/imgui_impl_glfw.cpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

glm::ivec2 UserInterface::viewportSize = glm::ivec2(1);
glm::ivec2 UserInterface::viewportScreenPosition = glm::ivec2(0);
bool UserInterface::interacting = false;

glm::ivec2 UserInterface::GetViewportSize() { return viewportSize; }
void UserInterface::UpdateViewportSize(const glm::ivec2 &newSize) { viewportSize = newSize; }

CallbackRef<int> UserInterface::onMouseButtonDownCallback;
bool UserInterface::clickedInViewport = false;

glm::ivec2 UserInterface::GetViewportScreenPosition()
{
    return viewportScreenPosition;
}

bool UserInterface::UpdateUserInteraction(bool interacting)
{
    UserInterface::interacting |= interacting;
    return interacting;
}

bool UserInterface::GetUserInteracting()
{
    return interacting;
}

bool UserInterface::GetClickedInViewport()
{
    return clickedInViewport;
}

void UserInterface::ResetUserInteracting()
{
    interacting = false;
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
    io.ConfigWindowsMoveFromTitleBarOnly = true;

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

    // events
    onMouseButtonDownCallback = MakeCallbackRef<int>([&](int button) { OnMouseButtonDown(button); });
    Input::OnMouseButtonPressedSubscribe(onMouseButtonDownCallback);
}

void UserInterface::SetupDockspaceViewport()
{
    // setup dockspace window
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // setup dockspace style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // setup window flags
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // create the dockspace
    ImGui::Begin("Dockspace", NULL, windowFlags);
    ImGui::PopStyleVar();
    ImGui::DockSpace(ImGui::GetID("Main Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    ImGui::End();
    ImGui::PopStyleVar(2);

    ImGui::Begin("Viewport");
    // update viewport screen position
    auto regionMin = ImGui::GetWindowContentRegionMin();
    auto winPos = ImGui::GetWindowPos();
    viewportScreenPosition = glm::ivec2(regionMin.x + winPos.x, regionMin.y + winPos.y);

    // update viewport size
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    UserInterface::UpdateViewportSize(glm::ivec2(glm::max(0, (int)availableSize.x), glm::max(0, (int)availableSize.y)));

    // display the framebuffer
    ImGui::Image((void *)(intptr_t)Window::GetFramebufferTexture(), availableSize);
    ImGui::End();
}

void UserInterface::OnMouseButtonDown(int button)
{
    if (button == MOUSE_BUTTON_LEFT)
        clickedInViewport = Input::IsMouseInViewport() && !GetUserInteracting();
}

void UserInterface::FrameBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    SetupDockspaceViewport();
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
