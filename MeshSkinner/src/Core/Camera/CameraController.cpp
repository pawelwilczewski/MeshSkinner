#include "pch.h"
#include "CameraController.h"

static CallbackNoArgRef onUpdateCallback;
static CallbackNoArgRef onLateUpdateCallback;
static CallbackRef<int> onKeyPressedCallback;

CameraController::CameraController(Ref<Camera> camera, float moveSpeed) : camera(camera), moveSpeed(moveSpeed)
{
	onUpdateCallback = MakeCallbackNoArgRef([&]() { OnUpdate(); });
	onLateUpdateCallback = MakeCallbackNoArgRef([&]() { OnLateUpdate(); });
	onKeyPressedCallback = MakeCallbackRef<int>([&](int key) { OnKeyPressed(key); });

	Application::OnUpdateSubscribe(onUpdateCallback);
	Application::OnLateUpdateSubscribe(onLateUpdateCallback);
	Input::OnKeyPressedSubscribe(onKeyPressedCallback);
}

CameraController::~CameraController()
{
	Application::OnUpdateUnsubscribe(onUpdateCallback);
	Application::OnLateUpdateUnsubscribe(onLateUpdateCallback);
	Input::OnKeyPressedUnsubscribe(onKeyPressedCallback);
}

void CameraController::OnUpdate()
{
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
		auto forward = input.y * -camera->transform.GetRightVector();
		auto right = input.x * camera->transform.GetForwardVector();

		camera->transform.SetPosition(camera->transform.GetPosition() + moveSpeed * Time::GetDeltaSeconds() * (forward + right));
	}

	// rotation
	auto pos = Input::GetMousePosition();
	if (initialMouseMove)
	{
		lastMousePos = pos;
		initialMouseMove = false;
	}
	auto delta = pos - lastMousePos;
	lastMousePos = pos;
	camera->transform.SetRotation(camera->transform.GetRotation() + glm::vec3(delta.y, -delta.x, 0.f) * 0.05f);
}

void CameraController::OnLateUpdate()
{
	
}

void CameraController::OnKeyPressed(int key)
{
	if (key == KEY_SPACE)
	{
		active = !active;
		Window::SetCursorVisibility(!active);
		if (active)
			initialMouseMove = true;
	}
}
