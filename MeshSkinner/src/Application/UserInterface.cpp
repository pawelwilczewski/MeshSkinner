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
EventNoArg UserInterface::onDrawAdditionalViewportWidgets = EventNoArg();
ImFont *UserInterface::defaultFont = nullptr;
bool UserInterface::interacting = false;
bool UserInterface::clickedInViewport = false;
CallbackRef<int> UserInterface::onMouseButtonDownCallback;

void UserInterface::UpdateViewportSize(const glm::ivec2 &newSize)
{
	viewportSize = newSize;
}

glm::ivec2 UserInterface::GetViewportSize()
{
	return viewportSize;
}

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

static void SetupImGuiStyle()
{
	// MeshSkinnerDark style from ImThemes
	ImGuiStyle &style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5708154439926147f, 0.5708097219467163f, 0.5708097219467163f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1330472230911255f, 0.1330458968877792f, 0.1330458968877792f, 0.9399999976158142f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(9.999999974752427e-07f, 9.999899930335232e-07f, 9.999899930335232e-07f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1330472230911255f, 0.1330458968877792f, 0.1330458968877792f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.4205966293811798f, 0.4205966293811798f, 0.4206008315086365f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2060065567493439f, 0.2060076743364334f, 0.2060086131095886f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.296134352684021f, 0.2961360216140747f, 0.2961373329162598f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.412013053894043f, 0.4120152592658997f, 0.4120171666145325f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1115880012512207f, 0.1115868836641312f, 0.1115868836641312f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1459227204322815f, 0.1459212601184845f, 0.1459212601184845f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1630901098251343f, 0.163088470697403f, 0.163088470697403f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(9.999999974752427e-07f, 9.999899930335232e-07f, 9.999899930335232e-07f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.7772924304008484f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2060065567493439f, 0.2060076743364334f, 0.2060086131095886f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3819704055786133f, 0.3819724917411804f, 0.3819742202758789f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5450589060783386f, 0.5450618267059326f, 0.545064389705658f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1115880012512207f, 0.1115868836641312f, 0.1115868836641312f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1802574992179871f, 0.1802556961774826f, 0.1802556961774826f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(9.999899930335232e-07f, 9.999899930335232e-07f, 9.999999974752427e-07f, 0.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(9.999899930335232e-07f, 9.999899930335232e-07f, 9.999999974752427e-07f, 0.1459227204322815f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1673803329467773f, 0.1673803329467773f, 0.1673820018768311f, 0.1072961091995239f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2060065567493439f, 0.2060076743364334f, 0.2060086131095886f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.3819704055786133f, 0.3819724917411804f, 0.3819742202758789f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5450589060783386f, 0.5450618267059326f, 0.545064389705658f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1545064449310303f, 0.1545048952102661f, 0.1545048952102661f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2489270567893982f, 0.2489245682954788f, 0.2489245682954788f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.1888412237167358f, 0.1888393312692642f, 0.1888393312692642f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.2231759428977966f, 0.2231737077236176f, 0.2231737077236176f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2575107216835022f, 0.2575081288814545f, 0.2575081288814545f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.7081544995307922f, 0.708147406578064f, 0.708147406578064f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.7772925496101379f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.2832589447498322f, 0.2832589447498322f, 0.283261775970459f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274655550718307f, 0.2274655550718307f, 0.2274678349494934f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(9.999999974752427e-07f, 9.999899930335232e-07f, 9.999899930335232e-07f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 0.9999899864196777f, 0.9999899864196777f, 0.02145922183990479f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.5321835279464722f, 0.5321835279464722f, 0.5321888327598572f, 0.5f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

void UserInterface::Init()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // keyboard controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // multi-viewport
    io.ConfigWindowsMoveFromTitleBarOnly = true;

	defaultFont = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto-Regular.ttf", 18 * Window::GetScreenSize().y / 1440.f);

	SetupImGuiStyle();

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
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
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

    onDrawAdditionalViewportWidgets.Invoke();

    ImGui::End();
}

void UserInterface::OnDrawAdditionalViewportWidgetsSubscribe(const CallbackNoArgRef &callback)
{
    onDrawAdditionalViewportWidgets.Subscribe(callback);
}

void UserInterface::OnDrawAdditionalViewportWidgetsUnsubscribe(const CallbackNoArgRef &callback)
{
    onDrawAdditionalViewportWidgets.Unsubscribe(callback);
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

	ImGui::PushFont(defaultFont);

    SetupDockspaceViewport();
}

void UserInterface::FrameEnd()
{
	ImGui::PopFont();

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
