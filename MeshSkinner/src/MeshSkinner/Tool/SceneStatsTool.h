#pragma once

#include "Tool.h"

class SceneStatsTool : public Tool
{
public:
	SceneStatsTool(const std::string &toolWindowName = "Scene Stats");
	virtual ~SceneStatsTool() = default;

protected:
	virtual void OnUpdateUI() override;
};
