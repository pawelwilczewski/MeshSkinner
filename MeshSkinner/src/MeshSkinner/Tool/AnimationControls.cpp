#include "pch.h"
#include "AnimationControls.h"

#include "Hierarchy.h"

AnimationControls::AnimationControls(const std::string &toolWindowName) : Tool(toolWindowName)
{
	onUpdateCallback = MakeCallbackNoArgRef([&]() { OnUpdate(); });

	Application::OnUpdateSubscribe(onUpdateCallback);
}

AnimationControls::~AnimationControls()
{
	Application::OnUpdateUnsubscribe(onUpdateCallback);
}

const std::vector<Animation> &AnimationControls::GetAnimations() const
{
	auto mesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>().get();
	if (mesh)
		return animations.at(mesh).animations;

	return std::vector<Animation>();
}

void AnimationControls::OnUpdateUI()
{
	auto mesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>().get();
	if (!mesh)
		return;

	if (animations.find(mesh) == animations.end())
		animations.insert({ mesh, AnimationInfo() });

	auto &info = animations[mesh];

	ImGui::Begin(toolWindowName.c_str());

	InteractiveWidget(ImGui::InputText("Animations file path", &sourceFile)); // TODO: for text inputs: unfocus if clicked in the viewport

	auto dropped = Input::GetDroppedFiles();
	if (ImGui::IsItemHovered() && dropped && dropped->size() > 0)
		sourceFile = dropped->at(0);

	if (InteractiveWidget(ImGui::Button("Import animations")))
	{
		Log::Info("Importing animations from file {}", sourceFile);

		MeshLibrary::Import(sourceFile, info.animations);
	}

	std::vector<const char *> animationNames;
	for (const auto &animation : info.animations)
		animationNames.push_back(animation.name.c_str());

	if (InteractiveWidget(ImGui::ListBox("Select animation", &info.animationIndex, animationNames.data(), animationNames.size())))
		info.playbackTime = 0.f;

	if (info.animationIndex < info.animations.size())
	{
		auto &current = info.animations[info.animationIndex];

		InteractiveWidget(ImGui::Checkbox("Play", &info.play));
		InteractiveWidget(ImGui::Checkbox("Loop", &current.loop));

		ImGui::SliderFloat("Timeline", &info.playbackTime, 0.f, current.GetDuration());
	}

	ImGui::End();
}

void AnimationControls::OnUpdate()
{
	for (auto &[mesh, info] : animations)
	{
		if (info.animationIndex < info.animations.size())
		{
			auto &anim = info.animations[info.animationIndex];

			if (info.play)
				info.playbackTime = anim.GetTimeUsedForEvaluation(info.playbackTime + Time::GetDeltaSeconds());

			for (const auto &bone : mesh->skeleton->GetBones())
			{
				bone->transform.SetPosition(anim.EvaluateTranslation(bone->name, info.playbackTime));
				bone->transform.SetRotation(glm::degrees(glm::eulerAngles(anim.EvaluateRotation(bone->name, info.playbackTime))));
				bone->transform.SetScale(anim.EvaluateScale(bone->name, info.playbackTime));
			}
		}
	}
}
