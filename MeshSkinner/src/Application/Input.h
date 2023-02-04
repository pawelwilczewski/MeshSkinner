#pragma once

#include "Core.h"
#include "Event.h"
#include "InputCodes.h"

class Input
{
	friend class Application;

private:
	static void Init();

public:
	static bool IsKeyPressed(int key);
	static bool IsMouseButtonPressed(int button);
	static glm::vec2 GetMousePosition();

	static void OnKeyPressedSubscribe(CallbackRef<int> callback) { s_OnKeyPressed.Subscribe(callback); }
	static void OnKeyReleasedSubscribe(CallbackRef<int> callback) { s_OnKeyReleased.Subscribe(callback); }
	static void OnMouseButtonPressedSubscribe(CallbackRef<int> callback) { s_OnMouseButtonPressed.Subscribe(callback); }
	static void OnMouseButtonReleasedSubscribe(CallbackRef<int> callback) { s_OnMouseButtonReleased.Subscribe(callback); }
	static void OnMouseMovedSubscribe(CallbackRef<glm::vec2> callback) { s_OnMouseMoved.Subscribe(callback); }
	static void OnMouseScrolledSubscribe(CallbackRef<glm::vec2> callback) { s_OnMouseScrolled.Subscribe(callback); }
	static void OnWindowResizedSubscribe(CallbackRef<glm::ivec2> callback) { s_OnWindowResized.Subscribe(callback); }

	static void OnKeyPressedUnsubscribe(CallbackRef<int> callback) { s_OnKeyPressed.Unsubscribe(callback); }
	static void OnKeyReleasedUnsubscribe(CallbackRef<int> callback) { s_OnKeyReleased.Unsubscribe(callback); }
	static void OnMouseButtonPressedUnsubscribe(CallbackRef<int> callback) { s_OnMouseButtonPressed.Unsubscribe(callback); }
	static void OnMouseButtonReleasedUnsubscribe(CallbackRef<int> callback) { s_OnMouseButtonReleased.Unsubscribe(callback); }
	static void OnMouseMovedUnsubscribe(CallbackRef<glm::vec2> callback) { s_OnMouseMoved.Unsubscribe(callback); }
	static void OnMouseScrolledUnsubscribe(CallbackRef<glm::vec2> callback) { s_OnMouseScrolled.Unsubscribe(callback); }
	static void OnWindowResizedUnsubscribe(CallbackRef<glm::ivec2> callback) { s_OnWindowResized.Unsubscribe(callback); }

private:
	static void HandleKeyCallback(GLFWwindow *window, int key, int, int action, int);
	static void HandleMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void HandleMouseMovedCallback(GLFWwindow *window, double x, double y);
	static void HandleMouseScrolledCallback(GLFWwindow *window, double offsetX, double offsetY);
	static void HandleWindowResizedCallback(GLFWwindow *window, int width, int height);

private:
	static Event<int> s_OnKeyPressed;
	static Event<int> s_OnKeyReleased;
	static Event<int> s_OnMouseButtonPressed;
	static Event<int> s_OnMouseButtonReleased;
	static Event<glm::vec2> s_OnMouseMoved;
	static Event<glm::vec2> s_OnMouseScrolled;
	static Event<glm::ivec2> s_OnWindowResized;
};
