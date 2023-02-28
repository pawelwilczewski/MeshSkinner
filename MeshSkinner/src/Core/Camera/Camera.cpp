#include "pch.h"
#include "Camera.h"

static CallbackRef<glm::ivec2> onWindowResizedCallback;
static CallbackNoArgRef onStartCallback;

Camera::Camera(Transform transform, ProjectionMode projectionMode, float perspectiveVertFOV, float perspectiveNearClip, float perspectiveFarClip, float orthographicHeight) : transform(transform), projectionMode(projectionMode), perspectiveVertFOV(perspectiveVertFOV), perspectiveNearClip(perspectiveNearClip), perspectiveFarClip(perspectiveFarClip), orthographicHeight(orthographicHeight)
{
	// setup callbacks
	onWindowResizedCallback = MakeCallbackRef<glm::ivec2>([&](const glm::ivec2 &newSize) { OnWindowResized(newSize); });

	// event subscription
	Input::OnWindowResizedSubscribe(onWindowResizedCallback);

	RecalculateViewProjection();
}

Camera::~Camera()
{
	// event unsubscription
	Input::OnWindowResizedUnsubscribe(onWindowResizedCallback);
}

const glm::mat4 &Camera::GetViewProjectionMatrix()
{
	if (!isViewProjectionUpdated || !transform.IsMatrixUpdated())
		RecalculateViewProjection();

	return viewProjectionMatrix;
}

void Camera::OnWindowResized(const glm::ivec2 &newSize)
{
	isViewProjectionUpdated = false;
}

void Camera::RecalculateViewProjection()
{
	glm::mat4 projection;
	auto viewportSize = UserInterface::GetViewportSize();
	auto aspectRatio = (float)viewportSize.x / viewportSize.y;
	switch (projectionMode)
	{
		case ProjectionMode::Perspective:
			// NOTE: the negative FOV because of rendering to the framebuffer - the image is flipped otherwise
			projection = glm::perspective(-perspectiveVertFOV, aspectRatio, perspectiveNearClip, perspectiveFarClip);
			break;
		case ProjectionMode::Orthographic:
			auto halfHeight = orthographicHeight * 0.5f;
			projection = glm::ortho(-halfHeight * aspectRatio, halfHeight * aspectRatio, halfHeight, -halfHeight);
			break;
	}
	auto view = glm::lookAt(transform.GetPosition(), transform.GetPosition() + -transform.GetRightVector(), transform.GetUpVector());

	viewProjectionMatrix = projection * view;
	isViewProjectionUpdated = true;
}

void Camera::SetProjectionMode(ProjectionMode newMode)
{
	projectionMode = newMode;
	isViewProjectionUpdated = false;
}

Camera::ProjectionMode Camera::GetViewProjectionMode() const
{
	return projectionMode;
}

Ray Camera::ProjectViewportToWorld(const glm::vec2 &viewportPos)
{
	// convert mouse pos to [-1, 1] space
	auto mouseCoords = viewportPos / (glm::vec2(UserInterface::GetViewportSize()) * 0.5f) - 1.f;

	glm::vec4 screenPos = glm::vec4(mouseCoords.x, mouseCoords.y, 1.0f, 1.0f);
	glm::vec4 worldPos = glm::inverse(GetViewProjectionMatrix()) * screenPos;
	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

	Log::Info("{}", screenPos);

	return Ray(transform.GetPosition(), dir);
}
