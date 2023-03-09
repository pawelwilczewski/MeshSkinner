#include "pch.h"
#include "AnimationControls.h"

AnimationControls::AnimationControls(const std::string &toolWindowName) : Tool(toolWindowName)
{
}

const std::vector<Animation> &AnimationControls::GetAnimations() const
{
	return animations;
}

Animation *AnimationControls::GetCurrentAnimation()
{
	if (animationIndex < animations.size())
		return &animations[animationIndex];
	
	return nullptr;
}

float AnimationControls::GetAnimationTime() const
{
	return animationTime;
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

		MeshLibrary::Import(sourceFile, animations);

		animationNames.clear();
		for (const auto &animation : animations)
			animationNames.push_back(animation.name.c_str());

		animationIndex = 0;
		animationTime = 0.f;
	}

	if (InteractiveWidget(ImGui::ListBox("Select animation", &animationIndex, animationNames.data(), animationNames.size())))
		animationTime = 0.f;

	auto current = GetCurrentAnimation();
	if (current)
	{
		InteractiveWidget(ImGui::Checkbox("Play back", &playBack));
		InteractiveWidget(ImGui::Checkbox("Loop", &current->loop));

		if (playBack)
		{
			animationTime += Time::GetDeltaSeconds();
			animationTime = current->GetTimeUsedForEvaluation(animationTime);
		}

		ImGui::SliderFloat("Timeline", &animationTime, 0.f, current->GetDuration());
	}

	ImGui::End();
}
