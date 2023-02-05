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
	static GLFWwindow *GetNativeWindow() { return window; }
	static glm::ivec2 GetFramebufferSize() { int width, height; glfwGetFramebufferSize(window, &width, &height); return { width, height }; }
	static GLuint GetFramebufferTexture() { return framebufferTexture; }
	static bool IsPendingClose() { return glfwWindowShouldClose(window); }

private:
	static GLFWwindow *window;
	static GLuint framebufferTexture;
};
