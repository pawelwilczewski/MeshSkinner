#pragma once

class UserInterface
{
	friend class Application;

public:
	static glm::ivec2 GetViewportSize();
	static void UpdateViewportSize(const glm::ivec2 &newSize);

private:
	static void Init();
	static void FrameBegin();
	static void UpdateUI();
	static void FrameEnd();
	static void Terminate();

private:
	static glm::ivec2 viewportSize;
};
