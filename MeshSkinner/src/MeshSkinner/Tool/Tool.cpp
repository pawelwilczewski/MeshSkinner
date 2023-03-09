#include "pch.h"
#include "Tool.h"

Tool::Tool(const std::string &toolWindowName) : toolWindowName(toolWindowName)
{
	onUpdateUICallback = MakeCallbackNoArgRef([&]() { OnUpdateUI(); });

	Application::OnUpdateUISubscribe(onUpdateUICallback);
}

Tool::~Tool()
{
	Application::OnUpdateUIUnsubscribe(onUpdateUICallback);
}
