#pragma once

#include "Tool.h"

class CameraControllerComponent;

class SettingsTool : public Tool
{
public:
	SettingsTool(const std::string &toolWindowName = "Settings", CameraControllerComponent *controller = nullptr);
	virtual ~SettingsTool() = default;

protected:
	virtual void OnUpdateUI() override;

public:
	CameraControllerComponent *cameraController = nullptr;
};
