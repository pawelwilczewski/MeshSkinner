#include "pch.h"
#include "Camera.h"

Camera::Camera(Transform transform) : transform(transform)
{
	// setup callbacks
	onWindowResizedCallback = MakeCallbackRef<glm::ivec2>([&](const glm::ivec2 &newSize) { OnWindowResized(newSize); });
	onStartCallback = MakeCallbackNoArgRef([&]() { OnStart(); });

	// event subscription
	Application::OnStartSubscribe(onStartCallback);
	Input::OnWindowResizedSubscribe(onWindowResizedCallback);
}

Camera::~Camera()
{
	// event unsubscription
	Application::OnStartUnsubscribe(onStartCallback);
	Input::OnWindowResizedUnsubscribe(onWindowResizedCallback);
}

const glm::mat4 &Camera::GetViewProjectionMatrix() const
{
	return viewProjectionMatrix;
}

void Camera::OnStart()
{
	Log::Trace("Camera start");
}

void Camera::OnWindowResized(const glm::ivec2 &newSize)
{
	Log::Trace("Camera window resized {0}", newSize);
}

inline void Camera::UpdateViewProjection()
{

}
