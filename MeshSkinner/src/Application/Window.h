#pragma once

class Window
{
	friend class Application;

private:
	static void Init(const glm::ivec2 &windowSize, const char *title, int vsync);
	static void FrameBegin();
	static void FrameEnd();
	static void Terminate();

public:
	static GLFWwindow *GetNativeWindow();
	static glm::ivec2 GetFramebufferSize();
	static GLuint GetFramebufferTexture();
	static bool IsPendingClose();

public:
	static void SetCursorVisibility(bool visible);

private:
	static GLFWwindow *window;
	static GLuint framebufferTexture;
};
