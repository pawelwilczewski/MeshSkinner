#pragma once

#include "Application/Core.h"
#include "Tool.h"

class SceneStats : public Tool
{
public:
	SceneStats(const std::string &toolWindowName = "Scene Stats");
	virtual ~SceneStats() = default;

protected:
	virtual void OnUpdateUI() override;
};
