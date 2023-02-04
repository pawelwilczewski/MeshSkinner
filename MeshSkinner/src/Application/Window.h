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
	static glm::ivec2 GetFramebufferSize() { int width, height; glfwGetFramebufferSize(s_Window, &width, &height); return { width, height }; }
	static GLuint GetFramebufferTexture() { return s_FramebufferTexture; }
	static bool IsPendingClose() { return glfwWindowShouldClose(s_Window); }

private:
	static GLFWwindow *s_Window;
	static GLuint s_FramebufferTexture;
};
