#pragma once

class Application
{
public:
	Application(uint32_t width, uint32_t height, const char *title);

	void Run();

private:
	GLFWwindow *m_Window;
};
