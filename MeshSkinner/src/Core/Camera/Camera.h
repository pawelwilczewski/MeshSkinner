#pragma once

#include "Core/Utils/MathUtils.h"

class Camera : public Entity
{
	enum class ProjectionMode
	{
		Perspective, Orthographic
	};

public:
	Camera(const std::string &name, Transform transform = Transform(), ProjectionMode projectionMode = ProjectionMode::Perspective, float perspectiveVertFOV = 45.f, float perspectiveNearClip = 0.1f, float perspectiveFarClip = 10000.f, float orthographicHeight = 10.f);
	virtual ~Camera();

public:
	const glm::mat4 &GetViewProjectionMatrix();

	void SetProjectionMode(ProjectionMode newMode);
	ProjectionMode GetViewProjectionMode() const;

public:
	MathUtils::Ray ProjectViewportToWorld(const glm::vec2 &viewportPos);
	glm::vec2 DeprojectWorldToViewport(const glm::vec3 &worldPos);

protected:
	virtual void OnWindowResized(const glm::ivec2 &newSize);
	virtual void RecalculateViewProjection();

protected:
	glm::mat4 viewProjectionMatrix;
	ProjectionMode projectionMode;

	bool isViewProjectionUpdated = false;

	float perspectiveVertFOV, perspectiveNearClip, perspectiveFarClip;
	float orthographicHeight;
};
