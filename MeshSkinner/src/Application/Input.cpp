#include "pch.h"
#include "Input.h"

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

glm::vec2 Input::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(Window::GetNativeWindow(), &x, &y);
	return { (float)x, (float)y };
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
	onMouseMoved.Invoke({ (float)x, (float)y });
}

void Input::HandleMouseScrolledCallback(GLFWwindow *window, double offsetX, double offsetY)
{
	onMouseScrolled.Invoke({ (float)offsetX, (float)offsetY });
}

void Input::HandleWindowResizedCallback(GLFWwindow *window, int width, int height)
{
	onWindowResized.Invoke({ width, height });
}
