#pragma once

#include "Application/Core.h"
#include "Tool.h"

class CameraControllerComponent;

class Settings : public Tool
{
public:
	Settings(const std::string &toolWindowName = "Settings", CameraControllerComponent *controller = nullptr);
	virtual ~Settings() = default;

protected:
	virtual void OnUpdateUI() override;

public:
	CameraControllerComponent *cameraController = nullptr;

public:
	inline static float tipBoneLength = 50.f;
	inline static float boneRadius = 5.f;
};
