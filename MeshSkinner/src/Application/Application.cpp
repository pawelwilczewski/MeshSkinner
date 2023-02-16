#include "pch.h"
#include "Application.h"

static EventNoArg onStart;
static EventNoArg onEarlyUpdate;
static EventNoArg onUpdate;
static EventNoArg onUpdateUI;
static EventNoArg onLateUpdate;
static EventNoArg onEnd;

void Application::Init(const glm::ivec2 &windowSize, const char *title, int vsync)
{
    Log::Init();
    Time::Init();
    Window::Init(windowSize, title, vsync);
    Input::Init();
    UserInterface::Init();
    Renderer::Init();
    ShaderLibrary::Init();
    MaterialLibrary::Init();

    Log::Trace("Application initialized...");
}

void Application::Run()
{
    Log::Trace("Application starting...");

    Start();

    while (!Window::IsPendingClose())
    {
        Time::FrameBegin();
        Window::FrameBegin();
        UserInterface::FrameBegin();
        Renderer::FrameBegin();

        EarlyUpdate();
        Update();
        UpdateUI();
        LateUpdate();

        Renderer::FrameEnd();
        Window::FrameEnd();
        UserInterface::FrameEnd();
    }

    End();
    Terminate();
}

void Application::Terminate()
{
    UserInterface::Terminate();
    Window::Terminate();

    exit(EXIT_SUCCESS);
}

void Application::Start() { onStart.Invoke(); }
void Application::EarlyUpdate() { onEarlyUpdate.Invoke(); }
void Application::Update() { onUpdate.Invoke(); }
void Application::UpdateUI() { onUpdateUI.Invoke(); }
void Application::LateUpdate() { onLateUpdate.Invoke(); }
void Application::End() { onEnd.Invoke(); }

void Application::OnStartSubscribe(CallbackNoArgRef callback) { onStart.Subscribe(callback); }
void Application::OnEarlyUpdateSubscribe(CallbackNoArgRef callback) { onEarlyUpdate.Subscribe(callback); }
void Application::OnUpdateSubscribe(CallbackNoArgRef callback) { onUpdate.Subscribe(callback); }
void Application::OnUpdateUISubscribe(CallbackNoArgRef callback) { onUpdateUI.Subscribe(callback); }
void Application::OnLateUpdateSubscribe(CallbackNoArgRef callback) { onLateUpdate.Subscribe(callback); }
void Application::OnEndSubscribe(CallbackNoArgRef callback) { onEnd.Subscribe(callback); }

void Application::OnStartUnsubscribe(CallbackNoArgRef callback) { onStart.Unsubscribe(callback); }
void Application::OnEarlyUpdateUnsubscribe(CallbackNoArgRef callback) { onEarlyUpdate.Unsubscribe(callback); }
void Application::OnUpdateUnsubscribe(CallbackNoArgRef callback) { onUpdate.Unsubscribe(callback); }
void Application::OnUpdateUIUnsubscribe(CallbackNoArgRef callback) { onUpdateUI.Unsubscribe(callback); }
void Application::OnLateUpdateUnsubscribe(CallbackNoArgRef callback) { onLateUpdate.Unsubscribe(callback); }
void Application::OnEndUnsubscribe(CallbackNoArgRef callback) { onEnd.Unsubscribe(callback); }
