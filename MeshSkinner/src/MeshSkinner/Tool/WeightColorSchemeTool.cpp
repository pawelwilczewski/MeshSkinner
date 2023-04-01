#include "pch.h"
#include "WeightColorSchemeTool.h"

WeightColorSchemeTool::WeightColorSchemeTool(const std::string &toolWindowName) : Tool(toolWindowName)
{

}

static const char *schemes[] = { "Viridis", "Magma", "Rainbow" };

void WeightColorSchemeTool::OnUpdateUI()
{
	ImGui::Begin("Color scheme");

	if (InteractiveWidget(ImGui::ListBox("Color scheme", &colorSchemeIndex, schemes, 3)))
	{
		switch (colorSchemeIndex)
		{
		case 0:
			// viridis
			Renderer::color000 = glm::vec3(0.33f, 0.07f, 0.32f);
			Renderer::color025 = glm::vec3(0.23f, 0.37f, 0.55f);
			Renderer::color050 = glm::vec3(0.18f, 0.60f, 0.63f);
			Renderer::color075 = glm::vec3(0.06f, 0.65f, 0.24f);
			Renderer::color100 = glm::vec3(1.00f, 0.81f, 0.13f);
			break;
		case 1:
			// magma
			Renderer::color000 = glm::vec3(0.00f, 0.00f, 0.01f);
			Renderer::color025 = glm::vec3(0.25f, 0.07f, 0.45f);
			Renderer::color050 = glm::vec3(0.67f, 0.20f, 0.49f);
			Renderer::color075 = glm::vec3(0.98f, 0.51f, 0.37f);
			Renderer::color100 = glm::vec3(0.99f, 0.99f, 0.75f);
			break;
		case 2:
			// rainbow
			Renderer::color000 = glm::vec3(0.f, 0.f, 1.f);
			Renderer::color025 = glm::vec3(0.f, 1.f, 1.f);
			Renderer::color050 = glm::vec3(0.f, 1.f, 0.f);
			Renderer::color075 = glm::vec3(1.f, 1.f, 0.f);
			Renderer::color100 = glm::vec3(1.f, 0.f, 0.f);
			break;

		default:
			assert(false);
		}
	}

	InteractiveWidget(ImGui::ColorEdit3("Colour   0%", glm::value_ptr(Renderer::color000)));
	InteractiveWidget(ImGui::ColorEdit3("Colour  25%", glm::value_ptr(Renderer::color025)));
	InteractiveWidget(ImGui::ColorEdit3("Colour  50%", glm::value_ptr(Renderer::color050)));
	InteractiveWidget(ImGui::ColorEdit3("Colour  75%", glm::value_ptr(Renderer::color075)));
	InteractiveWidget(ImGui::ColorEdit3("Colour 100%", glm::value_ptr(Renderer::color100)));

	ImGui::End();
}
