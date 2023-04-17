#pragma once

#include "Tool.h"

class CameraControllerComponent;

class SettingsTool : public Tool
{
public:
	SettingsTool(const std::string &toolWindowName = "Settings", CameraControllerComponent *controller = nullptr);
	virtual ~SettingsTool();

protected:
	virtual void OnUpdateUI() override;

public:
	CameraControllerComponent *cameraController = nullptr;

private:
	int colorSchemeIndex = 0;

	Camera *camera = nullptr;

	CallbackNoArgRef onDrawAdditionalViewportWidgetsCallback;
};
