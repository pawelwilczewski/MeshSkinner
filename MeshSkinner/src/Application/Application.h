#pragma once

class Application
{
public:
	static void Init(uint32_t width, uint32_t height, const char *title);

	static void Run();

	static GLFWwindow *GetWindow() { return s_Window; }

private:
	static GLFWwindow *s_Window;
};
