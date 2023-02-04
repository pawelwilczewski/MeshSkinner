#include "pch.h"
#include "Input.h"

Event<int> Input::s_OnKeyPressed;
Event<int> Input::s_OnKeyReleased;
Event<int> Input::s_OnMouseButtonPressed;
Event<int> Input::s_OnMouseButtonReleased;
Event<glm::vec2> Input::s_OnMouseMoved;
Event<glm::vec2> Input::s_OnMouseScrolled;
Event<glm::ivec2> Input::s_OnWindowResized;

void Input::Init()
{
	glfwSetKeyCallback(Application::GetWindow(), &HandleKeyCallback);
	glfwSetCursorPosCallback(Application::GetWindow(), &HandleMouseMovedCallback);
	glfwSetMouseButtonCallback(Application::GetWindow(), &HandleMouseButtonCallback);
	glfwSetScrollCallback(Application::GetWindow(), &HandleMouseScrolledCallback);
	glfwSetWindowSizeCallback(Application::GetWindow(), &HandleWindowResizedCallback);
}

bool Input::IsKeyPressed(int key)
{
	auto state = glfwGetKey(Application::GetWindow(), key);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button)
{
	return glfwGetMouseButton(Application::GetWindow(), button) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(Application::GetWindow(), &x, &y);
	return { (float)x, (float)y };
}

void Input::HandleKeyCallback(GLFWwindow *window, int key, int, int action, int)
{
	if (action == GLFW_PRESS) s_OnKeyPressed.Invoke(key);
	else if (action == GLFW_RELEASE) s_OnKeyReleased.Invoke(key);
}

void Input::HandleMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) s_OnMouseButtonPressed.Invoke(button);
	else if (action == GLFW_RELEASE) s_OnMouseButtonReleased.Invoke(button);
}

void Input::HandleMouseMovedCallback(GLFWwindow *window, double x, double y)
{
	s_OnMouseMoved.Invoke({ (float)x, (float)y });
}

void Input::HandleMouseScrolledCallback(GLFWwindow *window, double offsetX, double offsetY)
{
	s_OnMouseScrolled.Invoke({ (float)offsetX, (float)offsetY });
}

void Input::HandleWindowResizedCallback(GLFWwindow *window, int width, int height)
{
	s_OnWindowResized.Invoke({ width, height });
}
