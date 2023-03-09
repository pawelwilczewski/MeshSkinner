#include "pch.h"
#include "AnimationControls.h"

AnimationControls::AnimationControls(const std::string &toolWindowName) : Tool(toolWindowName)
{
}

const std::vector<Animation> &AnimationControls::GetAnimations() const
{
	return animations;
}

void AnimationControls::OnUpdateUI()
{
	ImGui::Begin(toolWindowName.c_str());

	InteractiveWidget(ImGui::InputText("Animations file path", &sourceFile)); // TODO: for text inputs: unfocus if clicked in the viewport

	auto &dropped = Input::GetDroppedFiles();
	if (ImGui::IsItemHovered() && dropped.size() > 0)
		sourceFile = dropped[0];

	if (InteractiveWidget(ImGui::Button("Import animations")))
	{
		Log::Info("Importing animations from file {}", sourceFile);


	}

	//ImGui::ListBox
	//ImGui::SliderFloat("Timeline", 0.5f, 0.f, );

	ImGui::End();
}
