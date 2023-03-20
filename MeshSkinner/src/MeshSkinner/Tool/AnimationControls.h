#pragma once

#include "Application/Core.h"
#include "Tool.h"

struct AnimationInfo
{
	std::vector<Animation> animations;
	bool play = true;
	float playbackTime = 0.f;
	int animationIndex = 0;
};

class AnimationControls : public Tool
{
public:
	AnimationControls(const std::string &toolWindowName = "Animation Controls");
	virtual ~AnimationControls();

public:
	const std::vector<Animation> &GetAnimations() const;

protected:
	virtual void OnUpdateUI() override;
	void OnUpdate();

private:
	std::string sourceFile;

	std::unordered_map<SkeletalMeshComponent *, AnimationInfo> animations;
	CallbackNoArgRef onUpdateCallback;
};
