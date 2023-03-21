#pragma once

#include "Application/Core.h"
#include "Camera.h"

class CameraControllerComponent : public EntityComponent
{
public:
	CameraControllerComponent(const std::string &name = "CameraControllerComponent", float moveSpeed = 10.f, float moveSpeedMultiplier = 1.f, float moveSpeedMultiplierDelta = 0.2f, float maxSpeed = 10000.f, float minSpeed = 0.1f);
	virtual ~CameraControllerComponent();

protected:
	void OnUpdate();

	void OnMouseScrolled(const glm::vec2 &delta);
	void OnMouseMoved(const glm::vec2 &delta);

	virtual void OnAttached() override;
	virtual void OnDetached() override;

public:
	float moveSpeed;
	float mouseSensitivity = 0.05f;

protected:
	Camera *camera;
	bool active = false;

	float moveSpeedMultiplier;
	float moveSpeedMultiplierDelta;
	float maxSpeed;
	float minSpeed;

private:
	CallbackNoArgRef onUpdateCallback;
	CallbackRef<glm::vec2> onMouseScrolledCallback;
	CallbackRef<glm::vec2> onMouseMovedCallback;
};
