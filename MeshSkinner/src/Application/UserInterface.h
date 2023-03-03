#pragma once

class UserInterface
{
	friend class Application;

public:
	static glm::ivec2 GetViewportSize();
	static void UpdateViewportSize(const glm::ivec2 &newSize);

public:
	static glm::ivec2 GetViewportScreenPosition();

public:
	static void UpdateUserInteraction(bool interacting);
	static bool GetUserInteracting();

private:
	static void Init();
	static void FrameBegin();
	static void FrameEnd();
	static void Terminate();

private:
	static void ResetUserInteracting();

private:
	static void SetupDockspaceViewport();

private:
	static glm::ivec2 viewportSize;
	static glm::ivec2 viewportScreenPosition;

	static bool interacting;
};
