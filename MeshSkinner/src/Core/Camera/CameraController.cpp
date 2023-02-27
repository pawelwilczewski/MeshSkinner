#include "pch.h"
#include "CameraController.h"

CameraController::CameraController(const Ref<Camera> &camera, float moveSpeed, float moveSpeedMultiplier, float moveSpeedMultiplierDelta, float maxSpeed, float minSpeed) : camera(camera), moveSpeed(moveSpeed), moveSpeedMultiplier(moveSpeedMultiplier), moveSpeedMultiplierDelta(moveSpeedMultiplierDelta), maxSpeed(maxSpeed), minSpeed(minSpeed)
{
	onUpdateCallback = MakeCallbackNoArgRef([&]() { OnUpdate(); });
	onMouseScrolledCallback = MakeCallbackRef<glm::vec2>([&](const glm::vec2 &delta) { OnMouseScrolled(delta); });
	onMouseMovedCallback = MakeCallbackRef<glm::vec2>([&](const glm::vec2 &delta) { OnMouseMoved(delta); });

	Application::OnUpdateSubscribe(onUpdateCallback);
	Input::OnMouseScrolledSubscribe(onMouseScrolledCallback);
	Input::OnMouseMovedSubscribe(onMouseMovedCallback);

	// TODO: investigate why setting this rotation resolves initial camera jump on mouse move
	camera->transform.SetRotation(glm::vec3(0.f));
}

CameraController::~CameraController()
{
	Application::OnUpdateUnsubscribe(onUpdateCallback);
	Input::OnMouseScrolledUnsubscribe(onMouseScrolledCallback);
	Input::OnMouseMovedUnsubscribe(onMouseMovedCallback);
}

void CameraController::OnUpdate()
{
	active = Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
	Window::SetCursorVisibility(!active);
	if (!active) return;

	auto input = glm::vec2(0.f);
	if (Input::IsKeyPressed(KEY_W)) input += glm::vec2(0.f, 1.f);
	if (Input::IsKeyPressed(KEY_S)) input += glm::vec2(0.f, -1.f);
	if (Input::IsKeyPressed(KEY_D)) input += glm::vec2(1.f, 0.f);
	if (Input::IsKeyPressed(KEY_A)) input += glm::vec2(-1.f, 0.f);

	if (glm::length2(input) >= 0.0001f)
	{
		input = glm::normalize(input);

		// camera has messed up right and forward vectors in this coordinate system
		auto forward = -input.y * camera->transform.GetRightVector();
		auto right = -input.x * camera->transform.GetForwardVector();

		camera->transform.SetPosition(camera->transform.GetPosition() + moveSpeed * moveSpeedMultiplier * Time::GetDeltaSeconds() * (forward + right));
	}
}

void CameraController::OnMouseScrolled(const glm::vec2 &delta)
{
	moveSpeedMultiplier += delta.y * moveSpeedMultiplierDelta * glm::pow(moveSpeedMultiplier, 1.01f);
	moveSpeedMultiplier = glm::clamp(moveSpeedMultiplier, minSpeed, maxSpeed);
}

void CameraController::OnMouseMoved(const glm::vec2 &position)
{
	if (!active) return;

	// rotation
	auto delta = Input::GetMouseDelta();
	camera->transform.SetRotation(camera->transform.GetRotation() + glm::vec3(-delta.y, delta.x, 0.f) * 0.05f);
}
