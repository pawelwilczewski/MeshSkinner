#pragma once

#include "Application/Core.h"

class Tool
{
public:
	Tool(const std::string &toolWindowName);
	virtual ~Tool();

protected:
	virtual void OnUpdateUI() = 0;

protected:
	std::string toolWindowName;

private:
	CallbackNoArgRef onUpdateUICallback;
};
