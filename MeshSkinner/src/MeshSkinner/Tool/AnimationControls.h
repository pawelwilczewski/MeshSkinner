#pragma once

#include "Application/Core.h"
#include "Tool.h"

class AnimationControls : public Tool
{
public:
	AnimationControls(const std::string &toolWindowName = "Animation Controls");
	virtual ~AnimationControls() = default;

protected:
	virtual void OnUpdateUI() override;

private:
};
