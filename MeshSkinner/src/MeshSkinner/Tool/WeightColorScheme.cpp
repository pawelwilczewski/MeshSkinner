#include "pch.h"
#include "WeightColorScheme.h"

WeightColorScheme::WeightColorScheme(const std::string &toolWindowName) : Tool(toolWindowName)
{

}

void WeightColorScheme::OnUpdateUI()
{
	ImGui::Begin("Color scheme");
	ImGui::ColorEdit3("Colour   0%", glm::value_ptr(Renderer::color000));
	ImGui::ColorEdit3("Colour  25%", glm::value_ptr(Renderer::color025));
	ImGui::ColorEdit3("Colour  50%", glm::value_ptr(Renderer::color050));
	ImGui::ColorEdit3("Colour  75%", glm::value_ptr(Renderer::color075));
	ImGui::ColorEdit3("Colour 100%", glm::value_ptr(Renderer::color100));
	ImGui::End();
}
