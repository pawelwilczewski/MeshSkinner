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
	static glm::vec2 GetMouseWindowPosition();
	static glm::vec2 GetMouseScreenPosition();
	static glm::vec2 GetMouseViewportPosition();
	static glm::vec2 GetMouseDelta();

	static void OnKeyPressedSubscribe(CallbackRef<int> callback) { onKeyPressed.Subscribe(callback); }
	static void OnKeyReleasedSubscribe(CallbackRef<int> callback) { onKeyReleased.Subscribe(callback); }
	static void OnMouseButtonPressedSubscribe(CallbackRef<int> callback) { onMouseButtonPressed.Subscribe(callback); }
	static void OnMouseButtonReleasedSubscribe(CallbackRef<int> callback) { onMouseButtonReleased.Subscribe(callback); }
	static void OnMouseMovedSubscribe(CallbackRef<glm::vec2> callback) { onMouseMoved.Subscribe(callback); }
	static void OnMouseScrolledSubscribe(CallbackRef<glm::vec2> callback) { onMouseScrolled.Subscribe(callback); }
	static void OnWindowResizedSubscribe(CallbackRef<glm::ivec2> callback) { onWindowResized.Subscribe(callback); }

	static void OnKeyPressedUnsubscribe(CallbackRef<int> callback) { onKeyPressed.Unsubscribe(callback); }
	static void OnKeyReleasedUnsubscribe(CallbackRef<int> callback) { onKeyReleased.Unsubscribe(callback); }
	static void OnMouseButtonPressedUnsubscribe(CallbackRef<int> callback) { onMouseButtonPressed.Unsubscribe(callback); }
	static void OnMouseButtonReleasedUnsubscribe(CallbackRef<int> callback) { onMouseButtonReleased.Unsubscribe(callback); }
	static void OnMouseMovedUnsubscribe(CallbackRef<glm::vec2> callback) { onMouseMoved.Unsubscribe(callback); }
	static void OnMouseScrolledUnsubscribe(CallbackRef<glm::vec2> callback) { onMouseScrolled.Unsubscribe(callback); }
	static void OnWindowResizedUnsubscribe(CallbackRef<glm::ivec2> callback) { onWindowResized.Unsubscribe(callback); }

private:
	static void HandleKeyCallback(GLFWwindow *window, int key, int, int action, int);
	static void HandleMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void HandleMouseMovedCallback(GLFWwindow *window, double x, double y);
	static void HandleMouseScrolledCallback(GLFWwindow *window, double offsetX, double offsetY);
	static void HandleWindowResizedCallback(GLFWwindow *window, int width, int height);

private:
	static glm::vec2 mouseDelta;

private:
	static Event<int> onKeyPressed;
	static Event<int> onKeyReleased;
	static Event<int> onMouseButtonPressed;
	static Event<int> onMouseButtonReleased;
	static Event<glm::vec2> onMouseMoved;
	static Event<glm::vec2> onMouseScrolled;
	static Event<glm::ivec2> onWindowResized;
};
