#include "pch.h"
#include "Brush.h"

Brush::Brush(int blendMode, float weight, float radius, float falloff, float strength) :
    blendMode(blendMode), weight(weight), radius(radius), falloff(falloff), strength(strength)
{

}

bool Brush::DisplayUI(const char *windowName)
{
    bool interacting = false;

    ImGui::Begin(windowName);
    const char *items[] = { "Linear", "Add", "Multiply", "Gaussian", "Mix" }; // TODO: obviously do not have it fixed here like that
    interacting |= ImGui::ListBox("Brush blend mode", &blendMode, items, 5);

    interacting |= ImGui::SliderFloat("Brush weight", &weight, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput);
    interacting |= ImGui::SliderFloat("Brush strength", &strength, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput);

    interacting |= ImGui::DragFloat("Brush radius", &radius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic);
    interacting |= ImGui::DragFloat("Brush falloff", &falloff, 0.01f, 0.f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput);

    ImGui::End();

    return interacting;
}
