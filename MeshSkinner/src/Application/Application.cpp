#include "pch.h"
#include "Application.h"

Unique<Scene> Application::scene;

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
}

void Application::EarlyUpdate()
{
    scene->EarlyUpdate();
}

void Application::Update()
{
    scene->Update();
}

void Application::UpdateUI()
{
    scene->UpdateUI();
}

void Application::LateUpdate()
{
    scene->LateUpdate();
}
