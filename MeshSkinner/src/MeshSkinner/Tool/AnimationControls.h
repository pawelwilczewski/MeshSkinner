#pragma once

#include "Application/Core.h"
#include "Tool.h"

class AnimationControls : public Tool
{
public:
	AnimationControls(const std::string &toolWindowName = "Animation Controls");
	virtual ~AnimationControls() = default;

public:
	const std::vector<Animation> &GetAnimations() const;

	Animation *GetCurrentAnimation();
	float GetPlaybackTime() const;

protected:
	virtual void OnUpdateUI() override;

private:
	std::string sourceFile;

	std::vector<Animation> animations;
	std::vector<const char *> animationNames;

	bool play = true;
	float playbackTime = 0.f;
	int animationIndex = 0;
};
