#pragma once

#include "Application/Core.h"
#include "Core/Entity/Transform.h"
#include "Utils/MathUtils.h"

class Camera
{
	enum class ProjectionMode
	{
		Perspective, Orthographic
	};

public:
	Camera(Transform transform = Transform(), ProjectionMode projectionMode = ProjectionMode::Perspective, float perspectiveVertFOV = 45.f, float perspectiveNearClip = 0.1f, float perspectiveFarClip = 1000.f, float orthographicHeight = 10.f);
	virtual ~Camera();

public:
	const glm::mat4 &GetViewProjectionMatrix();

	void SetProjectionMode(ProjectionMode newMode);
	ProjectionMode GetViewProjectionMode() const;

public:
	Ray ProjectViewportToWorld(const glm::vec2 &viewportPos);

protected:
	virtual void OnWindowResized(const glm::ivec2 &newSize);
	virtual void RecalculateViewProjection();

public:
	Transform transform;

protected:
	glm::mat4 viewProjectionMatrix;
	ProjectionMode projectionMode;

	bool isViewProjectionUpdated = false;

	float perspectiveVertFOV, perspectiveNearClip, perspectiveFarClip;
	float orthographicHeight;
};
