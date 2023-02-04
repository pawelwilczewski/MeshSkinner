#pragma once

class Application
{
public:
	static void Init(uint32_t width, uint32_t height, const char *title);
	static void Run();

public:
	static GLFWwindow *GetWindow() { return s_Window; }
	static glm::ivec2 GetFrameBufferSize();

private:
	static void Start();
	static void Update();

private:
	static void InitImGui();
	static void UpdateImGui();

private:
	static GLFWwindow *s_Window;
};
