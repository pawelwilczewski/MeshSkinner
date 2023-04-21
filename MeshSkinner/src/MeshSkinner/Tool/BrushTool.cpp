#include "pch.h"
#include "BrushTool.h"

#include "MeshSkinner/Context.h"

static constexpr std::array<const char *, 12> BrushBlendModeNames = { "Mix", "Add", "Subtract", "Lighten", "Darken", "Color Dodge", "Difference", "Screen", "Hard Light", "Overlay", "Soft Light", "Exclusion" };

BrushTool::BrushTool(const std::string &toolWindowName, BlendMode blendMode, float weight, float radius, float falloff, float strength) :
    Tool(toolWindowName), blendMode(blendMode), weight(weight), radius(radius), falloff(falloff), strength(strength)
{
    onUpdateCallback = MakeCallbackNoArgRef([&]() { OnUpdate(); });
    Application::OnUpdateSubscribe(onUpdateCallback);

    onDrawAdditionalViewportWidgetsCallback = MakeCallbackNoArgRef([&]() {
        if (Window::GetCursorVisibility())
        {
            auto mousePos = Input::GetMouseScreenPosition();
            ImGui::GetWindowDrawList()->AddCircle(ImVec2(mousePos.x, mousePos.y), brushCircleSize, ImColor(0.8f, 0.8f, 0.8f, 1.f));
        }
        });
    UserInterface::OnDrawAdditionalViewportWidgetsSubscribe(onDrawAdditionalViewportWidgetsCallback);
}

BrushTool::~BrushTool()
{
    Application::OnUpdateUnsubscribe(onUpdateCallback);
    UserInterface::OnDrawAdditionalViewportWidgetsUnsubscribe(onDrawAdditionalViewportWidgetsCallback);
}

void BrushTool::OnUpdate()
{
    auto mesh = Context::Get().GetSelectedSkeletalMesh();

    if (mesh)
    {
        glm::vec3 intersect;
        if (MathUtils::RayMeshIntersection(camera->ProjectViewportToWorld(Input::GetMouseViewportPosition()), mesh, intersect))
        {
            auto offset = camera->transform.GetUpVector() * radius;
            auto screenPos = camera->DeprojectWorldToViewport(intersect + offset);
            auto mousePos = Input::GetMouseViewportPosition();
            brushCircleSize = glm::distance(mousePos, screenPos);
        }
    }
}

void BrushTool::OnUpdateUI()
{
    ImGui::Begin(toolWindowName.c_str());

    InteractiveWidget(ImGui::ListBox("Brush blend mode", (int *)(&blendMode), (const char **)BrushBlendModeNames.data(), (int)BrushBlendModeNames.size()));

    InteractiveWidget(ImGui::SliderFloat("Brush weight", &weight, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));
    //if (blendMode == BlendMode::Mix || blendMode == BlendMode::Linear)
        InteractiveWidget(ImGui::SliderFloat("Brush strength", &strength, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    InteractiveWidget(ImGui::DragFloat("Brush radius", &radius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
    InteractiveWidget(ImGui::DragFloat("Brush falloff", &falloff, 0.01f, 0.f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    ImGui::End();
}

float BrushTool::Blend(float oldWeight, float distance)
{
    auto alpha = glm::smoothstep(0.f, 1.f, glm::pow(1.f - distance / radius, falloff));

    float newWeight;
    switch (blendMode)
    {
    case BlendMode::Mix:        newWeight = MathUtils::BlendMix(oldWeight, weight, alpha); break;
    case BlendMode::Add:        newWeight = MathUtils::BlendAdd(oldWeight, weight, alpha); break;
    case BlendMode::Subtract:   newWeight = MathUtils::BlendSubtract(oldWeight, weight, alpha); break;
    case BlendMode::Lighten:    newWeight = MathUtils::BlendLighten(oldWeight, weight, alpha); break;
    case BlendMode::Darken:     newWeight = MathUtils::BlendDarken(oldWeight, weight, alpha); break;
    case BlendMode::ColorDodge: newWeight = MathUtils::BlendColorDodge(oldWeight, weight, alpha); break;
    case BlendMode::Difference: newWeight = MathUtils::BlendDifference(oldWeight, weight, alpha); break;
    case BlendMode::Screen:     newWeight = MathUtils::BlendScreen(oldWeight, weight, alpha); break;
    case BlendMode::HardLight:  newWeight = MathUtils::BlendHardLight(oldWeight, weight, alpha); break;
    case BlendMode::Overlay:    newWeight = MathUtils::BlendOverlay(oldWeight, weight, alpha); break;
    case BlendMode::SoftLight:  newWeight = MathUtils::BlendSoftLight(oldWeight, weight, alpha); break;
    case BlendMode::Exclusion:  newWeight = MathUtils::BlendExclusion(oldWeight, weight, alpha); break;
    default:                    assert(false);
    }

    return glm::clamp(oldWeight + (newWeight - oldWeight) * strength, 0.f, 1.f);
}
