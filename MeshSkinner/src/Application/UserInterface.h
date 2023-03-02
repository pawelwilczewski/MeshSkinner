#pragma once

class UserInterface
{
	friend class Application;

public:
	static glm::ivec2 GetViewportSize();
	static void UpdateViewportSize(const glm::ivec2 &newSize);

public:
	static glm::ivec2 GetViewportScreenPosition();

private:
	static void Init();
	static void FrameBegin();
	static void FrameEnd();
	static void Terminate();

private:
	static void SetupDockspaceViewport();

private:
	static glm::ivec2 viewportSize;
	static glm::ivec2 viewportScreenPosition;
};
