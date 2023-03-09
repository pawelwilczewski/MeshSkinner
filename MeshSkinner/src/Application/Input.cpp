#include "pch.h"
#include "Input.h"

glm::vec2 Input::mouseDelta = glm::vec2(0.f);

bool Input::droppedFilesInNewFrame = false;
std::vector<std::string> Input::droppedFiles;

CallbackNoArgRef Input::onEarlyUpdateCallback;
CallbackNoArgRef Input::onLateUpdateCallback;

Event<int> Input::onKeyPressed;
Event<int> Input::onKeyReleased;
Event<int> Input::onMouseButtonPressed;
Event<int> Input::onMouseButtonReleased;
Event<glm::vec2> Input::onMouseMoved;
Event<glm::vec2> Input::onMouseScrolled;
Event<glm::ivec2> Input::onWindowResized;
Event<std::vector<std::string>> Input::onFileDropped;

void Input::Init()
{
	glfwSetKeyCallback(Window::GetNativeWindow(), &HandleKeyCallback);
	glfwSetCursorPosCallback(Window::GetNativeWindow(), &HandleMouseMovedCallback);
	glfwSetMouseButtonCallback(Window::GetNativeWindow(), &HandleMouseButtonCallback);
	glfwSetScrollCallback(Window::GetNativeWindow(), &HandleMouseScrolledCallback);
	glfwSetWindowSizeCallback(Window::GetNativeWindow(), &HandleWindowResizedCallback);
	glfwSetDropCallback(Window::GetNativeWindow(), &HandleFileDropCallback);

	onEarlyUpdateCallback = MakeCallbackNoArgRef([&]() { OnEarlyUpdate(); });
	onLateUpdateCallback = MakeCallbackNoArgRef([&]() { OnLateUpdate(); });

	Application::OnEarlyUpdateSubscribe(onEarlyUpdateCallback);
	Application::OnLateUpdateSubscribe(onLateUpdateCallback);
}

bool Input::IsKeyPressed(int key)
{
	auto state = glfwGetKey(Window::GetNativeWindow(), key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	return glfwGetMouseButton(Window::GetNativeWindow(), button) == GLFW_PRESS;
}

glm::vec2 Input::GetMouseWindowPosition()
{
	double x, y;
	glfwGetCursorPos(Window::GetNativeWindow(), &x, &y);
	return { (float)x, (float)y };
}

glm::vec2 Input::GetMouseScreenPosition()
{
	return glm::vec2(Window::GetWindowPosition()) + GetMouseWindowPosition();
}

glm::vec2 Input::GetMouseViewportPosition()
{
	return GetMouseScreenPosition() - glm::vec2(UserInterface::GetViewportScreenPosition());
}

glm::vec2 Input::GetMouseDelta()
{
	return mouseDelta;
}

bool Input::IsMouseInViewport()
{
	auto pos = GetMouseViewportPosition();
	auto viewportSize = UserInterface::GetViewportSize();
	return pos.x >= 0 && pos.x < viewportSize.x && pos.y >= 0 && pos.y <= viewportSize.y;
}

void Input::OnKeyPressedSubscribe(const CallbackRef<int> &callback) { onKeyPressed.Subscribe(callback); }
void Input::OnKeyReleasedSubscribe(const CallbackRef<int> &callback) { onKeyReleased.Subscribe(callback); }
void Input::OnMouseButtonPressedSubscribe(const CallbackRef<int> &callback) { onMouseButtonPressed.Subscribe(callback); }
void Input::OnMouseButtonReleasedSubscribe(const CallbackRef<int> &callback) { onMouseButtonReleased.Subscribe(callback); }
void Input::OnMouseMovedSubscribe(const CallbackRef<glm::vec2> &callback) { onMouseMoved.Subscribe(callback); }
void Input::OnMouseScrolledSubscribe(const CallbackRef<glm::vec2> &callback) { onMouseScrolled.Subscribe(callback); }
void Input::OnWindowResizedSubscribe(const CallbackRef<glm::ivec2> &callback) { onWindowResized.Subscribe(callback); }
void Input::OnFileDroppedSubscribe(const CallbackRef<std::vector<std::string>> &callback) { onFileDropped.Subscribe(callback); }

void Input::OnKeyPressedUnsubscribe(const CallbackRef<int> &callback) { onKeyPressed.Unsubscribe(callback); }
void Input::OnKeyReleasedUnsubscribe(const CallbackRef<int> &callback) { onKeyReleased.Unsubscribe(callback); }
void Input::OnMouseButtonPressedUnsubscribe(const CallbackRef<int> &callback) { onMouseButtonPressed.Unsubscribe(callback); }
void Input::OnMouseButtonReleasedUnsubscribe(const CallbackRef<int> &callback) { onMouseButtonReleased.Unsubscribe(callback); }
void Input::OnMouseMovedUnsubscribe(const CallbackRef<glm::vec2> &callback) { onMouseMoved.Unsubscribe(callback); }
void Input::OnMouseScrolledUnsubscribe(const CallbackRef<glm::vec2> &callback) { onMouseScrolled.Unsubscribe(callback); }
void Input::OnWindowResizedUnsubscribe(const CallbackRef<glm::ivec2> &callback) { onWindowResized.Unsubscribe(callback); }
void Input::OnFileDroppedUnsubscribe(const CallbackRef<std::vector<std::string>> &callback) { onFileDropped.Unsubscribe(callback); }

void Input::HandleKeyCallback(GLFWwindow *window, int key, int, int action, int)
{
	if (action == GLFW_PRESS) onKeyPressed.Invoke(key);
	else if (action == GLFW_RELEASE) onKeyReleased.Invoke(key);
}

void Input::HandleMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) onMouseButtonPressed.Invoke(button);
	else if (action == GLFW_RELEASE) onMouseButtonReleased.Invoke(button);
}

void Input::HandleMouseMovedCallback(GLFWwindow *window, double x, double y)
{
	// having two frames of buffer before registering the input of the input ensures no sudden cursor jumps
	static int inputModeOneFrameAgo = GLFW_CURSOR_NORMAL;
	static int inputModeTwoFramesAgo = GLFW_CURSOR_NORMAL;
	static glm::vec2 lastMousePosition;

	// update the pos and input mode
	auto pos = glm::vec2(x, y);
	auto inputMode = glfwGetInputMode(window, GLFW_CURSOR);

	// calculate the delta
	if (inputMode != inputModeOneFrameAgo || inputMode != inputModeTwoFramesAgo)
		mouseDelta = glm::vec2(0.f);
	else
		mouseDelta = pos - lastMousePosition;

	// event invocation
	onMouseMoved.Invoke(pos);

	// update "last" variables
	lastMousePosition = pos;
	inputModeTwoFramesAgo = inputModeOneFrameAgo;
	inputModeOneFrameAgo = inputMode;
}

void Input::HandleMouseScrolledCallback(GLFWwindow *window, double offsetX, double offsetY)
{
	onMouseScrolled.Invoke({ (float)offsetX, (float)offsetY });
}

void Input::HandleWindowResizedCallback(GLFWwindow *window, int width, int height)
{
	onWindowResized.Invoke({ width, height });
}

void Input::HandleFileDropCallback(GLFWwindow *window, int pathCount, const char *paths[])
{
	droppedFiles.clear();
	for (int i = 0; i < pathCount; i++)
		droppedFiles.push_back(paths[i]);

	droppedFilesInNewFrame = false;

	onFileDropped.Invoke(droppedFiles);
}

void Input::OnEarlyUpdate()
{
	if (droppedFiles.size() > 0)
		droppedFilesInNewFrame = true;
}

void Input::OnLateUpdate()
{
	if (droppedFiles.size() > 0 && droppedFilesInNewFrame)
		droppedFiles.clear();

	droppedFilesInNewFrame = false;
}

std::vector<std::string> &Input::GetDroppedFiles()
{
	if (droppedFilesInNewFrame)
		return droppedFiles;
	else
		return std::vector<std::string>();
}
