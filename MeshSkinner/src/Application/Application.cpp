#include "pch.h"
#include "Application.h"

Unique<Scene> Application::scene;
EventNoArg Application::onStart;
EventNoArg Application::onEarlyUpdate;
EventNoArg Application::onUpdate;
EventNoArg Application::onUpdateUI;
EventNoArg Application::onLateUpdate;
EventNoArg Application::onEnd;

void Application::Init(uint32_t width, uint32_t height, const char *title, int vsync)
{
    Log::Init();
    Time::Init();
    Window::Init(width, height, title, vsync);
    Input::Init();
    UserInterface::Init();

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

        EarlyUpdate();
        Update();
        UpdateUI();
        LateUpdate();

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

void Application::Start()
{
    scene->Start();
    onStart.Invoke();
}

void Application::EarlyUpdate()
{
    scene->EarlyUpdate();
    onEarlyUpdate.Invoke();
}

void Application::Update()
{
    scene->Update();
    onUpdate.Invoke();
}

void Application::UpdateUI()
{
    scene->UpdateUI();
    onUpdateUI.Invoke();
}

void Application::LateUpdate()
{
    scene->LateUpdate();
    onLateUpdate.Invoke();
}

void Application::End()
{    
    scene->End();
    onEnd.Invoke();
}

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
