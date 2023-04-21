#include "pch.h"
#include "SceneStatsTool.h"

SceneStatsTool::SceneStatsTool(const std::string &toolWindowName) : Tool(toolWindowName)
{
}

void SceneStatsTool::OnUpdateUI()
{
    // debug fps info
    static float avgFps = 60.f;
    static float avgFrametime = 0.1667f;
    static float decayAlpha = 0.99f;

    avgFps = decayAlpha * avgFps + (1.f - decayAlpha) * Time::GetFPS();
    avgFrametime = decayAlpha * avgFrametime + (1.f - decayAlpha) * Time::GetDeltaSeconds();

    // scene stats
    ImGui::Begin("Scene Stats");
    ImGui::Text("FPS:            %f", Time::GetFPS());
    ImGui::Text("Frame time:     %f ms", Time::GetDeltaSeconds() * 1000.f);
    ImGui::Text("Avg FPS:        %f", avgFps);
    ImGui::Text("Avg frame time: %f ms", avgFrametime * 1000.f);
    ImGui::End();
}
