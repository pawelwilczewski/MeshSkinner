#pragma once

#include "Application/Core.h"
#include "Tool.h"

class WeightColorSchemeTool : public Tool
{
public:
	WeightColorSchemeTool(const std::string &toolWindowName = "Weight Color Scheme");
	virtual ~WeightColorSchemeTool() = default;

protected:
	virtual void OnUpdateUI() override;

	int colorSchemeIndex = 0;
};
