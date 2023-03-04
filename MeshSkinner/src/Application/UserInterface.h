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
	static bool UpdateUserInteraction(bool interacting);
	static bool GetUserInteracting();
	static bool GetClickedInViewport();

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
	static void OnMouseButtonDown(int button);

private:
	static glm::ivec2 viewportSize;
	static glm::ivec2 viewportScreenPosition;

	static bool interacting;

private:
	static CallbackRef<int> onMouseButtonDownCallback;
	static bool clickedInViewport;
};

constexpr auto InteractiveWidget = UserInterface::UpdateUserInteraction;
