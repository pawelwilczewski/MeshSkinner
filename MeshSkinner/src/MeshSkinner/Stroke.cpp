#include "pch.h"
#include "Stroke.h"

StrokeQueryInfo::StrokeQueryInfo(bool hitTarget, const glm::vec3 &worldPosition, const glm::vec2 &viewportPosition)
	: hitTarget(hitTarget), worldPosition(worldPosition), viewportPosition(viewportPosition)
{
}

Stroke::Stroke(const std::string &toolWindowName, Type type, float pixelDistance, float worldDistance, float fixedIntervalTime)
	: Tool(toolWindowName), type(type), pixelDistance(pixelDistance), worldDistance(worldDistance), fixedIntervalTime(fixedIntervalTime)
{
	switch (type)
	{
	case Type::EachFrame:
		break;
	case Type::FixedDeltaTime:
		break;
	case Type::PixelDistance:
		break;
	case Type::WorldDistance:
		break;
	default:
		assert(false);
	}
}

Stroke::~Stroke()
{

}

void Stroke::OnStrokeUpdate()
{
}

void Stroke::OnUpdateUI()
{
	ImGui::Begin(toolWindowName.c_str());

	const char *items[] = { "Pixel Distance", "World Distance", "Each Frame", "Fixed Delta Time" }; // TODO: obviously do not have it fixed here like that
	InteractiveWidget(ImGui::ListBox("Stroke update type", (int *)(&type), items, 4));

	switch (type)
	{
	case Type::PixelDistance:
		InteractiveWidget(ImGui::DragFloat("Pixel distance", &pixelDistance, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
		break;
	case Type::WorldDistance:
		InteractiveWidget(ImGui::DragFloat("World distance", &worldDistance, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
		break;
	case Type::EachFrame:
		break;
	case Type::FixedDeltaTime:
		InteractiveWidget(ImGui::DragFloat("Fixed delta time", &pixelDistance, 0.001f, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic));
		break;
	default:
		assert(false);
	}

	ImGui::End();
}
