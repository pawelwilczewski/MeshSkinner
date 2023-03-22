#pragma once

#include "Application/Core.h"
#include "Tool.h"

class Settings : public Tool
{
public:
	Settings(const std::string &toolWindowName = "Settings");
	virtual ~Settings() = default;

protected:
	virtual void OnUpdateUI() override;
};
