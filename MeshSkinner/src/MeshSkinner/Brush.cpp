#include "pch.h"
#include "Brush.h"

Brush::Brush(const std::string &toolWindowName, BlendMode blendMode, float weight, float radius, float falloff, float strength) :
    Tool(toolWindowName), blendMode(blendMode), weight(weight), radius(radius), falloff(falloff), strength(strength)
{

}

void Brush::OnUpdateUI()
{
    ImGui::Begin(toolWindowName.c_str());

    const char *items[] = { "Linear", "Add", "Multiply", "Gaussian", "Mix" }; // TODO: obviously do not have it fixed here like that
    UserInterface::UpdateUserInteraction(ImGui::ListBox("Brush blend mode", (int *)(&blendMode), items, 5));

    UserInterface::UpdateUserInteraction(ImGui::SliderFloat("Brush weight", &weight, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));
    if (blendMode == BlendMode::Mix || blendMode == BlendMode::Linear)
        UserInterface::UpdateUserInteraction(ImGui::SliderFloat("Brush strength", &strength, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    UserInterface::UpdateUserInteraction(ImGui::DragFloat("Brush radius", &radius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
    UserInterface::UpdateUserInteraction(ImGui::DragFloat("Brush falloff", &falloff, 0.01f, 0.f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    ImGui::End();
}

float Brush::Blend(float oldWeight, float newWeight)
{
    switch (blendMode)
    {
    case BlendMode::Mix:
        assert(false); // TODO: this one is somewhat wrong? unless too frequent updates make it appear so
        return (1.0f - strength) * oldWeight + strength * (newWeight - oldWeight);
    case BlendMode::Linear:
        return glm::mix(oldWeight, newWeight, strength);
    case BlendMode::Add:
        return glm::max(oldWeight, newWeight);
    case BlendMode::Multiply:
        return oldWeight * newWeight;
    case BlendMode::Gaussian:
        assert(false); // TODO: implement
        return 0.f;
    default:
        assert(false);
    }
}
