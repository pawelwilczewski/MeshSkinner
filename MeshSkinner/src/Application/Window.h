#pragma once

class Window
{
	friend class Application;

private:
	static void Init(int width, int height, const char *title, int vsync);
	static void FrameBegin();
	static void FrameEnd();
	static void Terminate();

public:
	static GLFWwindow *GetNativeWindow() { return s_Window; }
	static glm::ivec2 GetFramebufferSize();
	static GLuint GetFramebufferTexture() { return s_FramebufferTexture; }

private:
	static GLFWwindow *s_Window;
	static GLuint s_FramebufferTexture;
};
