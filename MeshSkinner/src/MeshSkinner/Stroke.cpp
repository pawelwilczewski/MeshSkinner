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

	const char *items[] = { "PixelDistance", "WorldDistance", "EachFrame", "FixedDeltaTime" }; // TODO: obviously do not have it fixed here like that
	InteractiveWidget(ImGui::ListBox("Stroke update type", (int *)(&type), items, 4));

	//InteractiveWidget(ImGui::SliderFloat("Brush weight", &weight, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));
	switch (type)
	{
	case Type::PixelDistance:
		float pixelDistance;
		float worldDistance;
		float fixedIntervalTime;
		break;
	case Type::WorldDistance:
		break;
	case Type::EachFrame:
		break;
	case Type::FixedDeltaTime:
		break;
	default:
		assert(false);
	}

	//if (type ::Mix || blendMode == BlendMode::Linear)
	//	interacting |= ImGui::SliderFloat("Brush strength", &strength, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput);

	//interacting |= ImGui::DragFloat("Brush radius", &radius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic);
	//interacting |= ImGui::DragFloat("Brush falloff", &falloff, 0.01f, 0.f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput);

	ImGui::End();
}
