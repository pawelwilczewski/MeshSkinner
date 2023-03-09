#include "pch.h"
#include "AnimationControls.h"

AnimationControls::AnimationControls(const std::string &toolWindowName) : Tool(toolWindowName)
{
}

void AnimationControls::OnUpdateUI()
{
	ImGui::Begin(toolWindowName.c_str());
	ImGui::Text("anim controls yeah");
	ImGui::End();
}
