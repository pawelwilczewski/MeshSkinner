#include "pch.h"
#include "Input.h"

glm::vec2 Input::mouseDelta = glm::vec2(0.f);

Event<int> Input::onKeyPressed;
Event<int> Input::onKeyReleased;
Event<int> Input::onMouseButtonPressed;
Event<int> Input::onMouseButtonReleased;
Event<glm::vec2> Input::onMouseMoved;
Event<glm::vec2> Input::onMouseScrolled;
Event<glm::ivec2> Input::onWindowResized;

void Input::Init()
{
	glfwSetKeyCallback(Window::GetNativeWindow(), &HandleKeyCallback);
	glfwSetCursorPosCallback(Window::GetNativeWindow(), &HandleMouseMovedCallback);
	glfwSetMouseButtonCallback(Window::GetNativeWindow(), &HandleMouseButtonCallback);
	glfwSetScrollCallback(Window::GetNativeWindow(), &HandleMouseScrolledCallback);
	glfwSetWindowSizeCallback(Window::GetNativeWindow(), &HandleWindowResizedCallback);

	//Application::OnUpdateSubscribe();
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
