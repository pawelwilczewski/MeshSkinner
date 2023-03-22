#pragma once

#include "Application/Core.h"
#include "Tool.h"

class WeightColorScheme : public Tool
{
public:
	WeightColorScheme(const std::string &toolWindowName = "Weight Color Scheme");
	virtual ~WeightColorScheme() = default;

protected:
	virtual void OnUpdateUI() override;
};
