#pragma once

#include "Transform.h"

class Camera
{
public:
	Camera(Transform transform = Transform());
	virtual ~Camera();

public:
	const glm::mat4 &GetViewProjectionMatrix() const;

protected:
	virtual void OnStart();
	virtual void OnWindowResized(const glm::ivec2 &newSize);

protected:
	virtual void UpdateViewProjection();

protected:
	Transform transform;
	glm::mat4 viewProjectionMatrix;

private:
	CallbackRef<glm::ivec2> onWindowResizedCallback;
	CallbackNoArgRef onStartCallback;
};
