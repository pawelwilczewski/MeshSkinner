#pragma once

#include "Application/Core.h"
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

public:
	inline static float tipBoneLength = 50.f;
	inline static float boneRadius = 5.f;
};
