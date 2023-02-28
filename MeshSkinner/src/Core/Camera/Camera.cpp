#include "pch.h"
#include "Camera.h"

static CallbackRef<glm::ivec2> onWindowResizedCallback;
static CallbackNoArgRef onStartCallback;

Camera::Camera(const std::string &name, Transform transform, ProjectionMode projectionMode, float perspectiveVertFOV, float perspectiveNearClip, float perspectiveFarClip, float orthographicHeight) : Entity(name, transform), projectionMode(projectionMode), perspectiveVertFOV(perspectiveVertFOV), perspectiveNearClip(perspectiveNearClip), perspectiveFarClip(perspectiveFarClip), orthographicHeight(orthographicHeight)
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
	if (!isViewProjectionUpdated || !GetIsWorldMatrixUpdated())
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
	auto viewportSize = UserInterface::GetViewportSize();

	glm::vec4 nearPos = glm::vec4(
		(viewportPos.x - viewportSize.x * 0.5f) / (viewportSize.x * 0.5f),
		(viewportPos.y - viewportSize.y * 0.5f) / (viewportSize.y * 0.5f), -1.f, 1.f);

	glm::vec4 farPos = glm::vec4(
		(viewportPos.x - viewportSize.x * 0.5f) / (viewportSize.x * 0.5f),
		(viewportPos.y - viewportSize.y * 0.5f) / (viewportSize.y * 0.5f), 1.f, 1.f);

	glm::mat4 invMat = glm::inverse(GetViewProjectionMatrix());
	glm::vec4 nearResult = invMat * nearPos;
	glm::vec4 farResult = invMat * farPos;
	nearResult /= nearResult.w;
	farResult /= farResult.w;

	glm::vec3 dir = glm::vec3(farResult - nearResult);

	return Ray(transform.GetPosition(), normalize(dir));
}
