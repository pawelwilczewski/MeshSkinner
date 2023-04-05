#pragma once

#include "Tool.h"

class AnimationControlsTool : public Tool
{
public:
	AnimationControlsTool(const std::string &toolWindowName = "Animation Controls");
	virtual ~AnimationControlsTool();

protected:
	virtual void OnUpdateUI() override;
	void OnUpdate();

private:
	std::string sourceFile;

	CallbackNoArgRef onUpdateCallback;
};
