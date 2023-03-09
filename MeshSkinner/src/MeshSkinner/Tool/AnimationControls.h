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

protected:
	virtual void OnUpdateUI() override;

private:
	std::string sourceFile;

	std::vector<Animation> animations;
};
