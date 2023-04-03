#pragma once

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

	static bool IsMouseInViewport();

	static void OnKeyPressedSubscribe(const CallbackRef<int> &callback);
	static void OnKeyReleasedSubscribe(const CallbackRef<int> &callback);
	static void OnMouseButtonPressedSubscribe(const CallbackRef<int> &callback);
	static void OnMouseButtonReleasedSubscribe(const CallbackRef<int> &callback);
	static void OnMouseMovedSubscribe(const CallbackRef<glm::vec2> &callback);
	static void OnMouseScrolledSubscribe(const CallbackRef<glm::vec2> &callback);
	static void OnWindowResizedSubscribe(const CallbackRef<glm::ivec2> &callback);
	static void OnFileDroppedSubscribe(const CallbackRef<std::vector<std::string>> &callback);

	static void OnKeyPressedUnsubscribe(const CallbackRef<int> &callback);
	static void OnKeyReleasedUnsubscribe(const CallbackRef<int> &callback);
	static void OnMouseButtonPressedUnsubscribe(const CallbackRef<int> &callback);
	static void OnMouseButtonReleasedUnsubscribe(const CallbackRef<int> &callback);
	static void OnMouseMovedUnsubscribe(const CallbackRef<glm::vec2> &callback);
	static void OnMouseScrolledUnsubscribe(const CallbackRef<glm::vec2> &callback);
	static void OnWindowResizedUnsubscribe(const CallbackRef<glm::ivec2> &callback);
	static void OnFileDroppedUnsubscribe(const CallbackRef<std::vector<std::string>> &callback);

	static const std::vector<std::string> *GetDroppedFiles();

private:
	static void HandleKeyCallback(GLFWwindow *window, int key, int, int action, int);
	static void HandleMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void HandleMouseMovedCallback(GLFWwindow *window, double x, double y);
	static void HandleMouseScrolledCallback(GLFWwindow *window, double offsetX, double offsetY);
	static void HandleWindowResizedCallback(GLFWwindow *window, int width, int height);
	static void HandleFileDropCallback(GLFWwindow *window, int pathCount, const char *paths[]);

private:
	static void OnEarlyUpdate();
	static void OnLateUpdate();	

private:
	static glm::vec2 mouseDelta;

	static std::vector<std::string> droppedFiles;
	static bool droppedFilesInNewFrame;

private:
	static CallbackNoArgRef onEarlyUpdateCallback;
	static CallbackNoArgRef onLateUpdateCallback;

private:
	static Event<int> onKeyPressed;
	static Event<int> onKeyReleased;
	static Event<int> onMouseButtonPressed;
	static Event<int> onMouseButtonReleased;
	static Event<glm::vec2> onMouseMoved;
	static Event<glm::vec2> onMouseScrolled;
	static Event<glm::ivec2> onWindowResized;
	static Event<std::vector<std::string>> onFileDropped;
};
