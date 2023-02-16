#pragma once

#include "Camera.h"

class CameraController
{
public:
	CameraController(Ref<Camera> camera, float moveSpeed = 10.f);
	virtual ~CameraController();

protected:
	void OnUpdate();
	void OnLateUpdate();

	void OnKeyPressed(int key);

public:
	float moveSpeed;

protected:
	Ref<Camera> camera;
	glm::vec2 lastMousePos = glm::vec2(0.f);
	bool active = false;
	bool initialMouseMove = true;
};
