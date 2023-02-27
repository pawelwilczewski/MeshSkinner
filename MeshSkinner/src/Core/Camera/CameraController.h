#pragma once

#include "Camera.h"

class CameraController
{
public:
	CameraController(const Ref<Camera> &camera, float moveSpeed = 10.f, float moveSpeedMultiplier = 1.f, float moveSpeedMultiplierDelta = 0.2f, float maxSpeed = 10000.f, float minSpeed = 0.1f);
	virtual ~CameraController();

protected:
	void OnUpdate();

	void OnMouseScrolled(const glm::vec2 &delta);
	void OnMouseMoved(const glm::vec2 &delta);

public:
	float moveSpeed;
	float mouseSensitivity = 0.05f;

protected:
	Ref<Camera> camera;
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
