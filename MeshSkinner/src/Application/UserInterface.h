#pragma once

class UserInterface
{
	friend class Application;

public:
	static glm::ivec2 GetViewportSize();
	static void UpdateViewportSize(const glm::ivec2 &newSize);
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

	static void ResetUserInteracting();

	static void SetupDockspaceViewport();

public:
	static void OnDrawAdditionalViewportWidgetsSubscribe(const CallbackNoArgRef &callback);
	static void OnDrawAdditionalViewportWidgetsUnsubscribe(const CallbackNoArgRef &callback);

private:
	static void OnMouseButtonDown(int button);

private:
	static glm::ivec2 viewportSize;
	static glm::ivec2 viewportScreenPosition;

	static bool interacting;

	static ImFont *defaultFont;

private:
	static CallbackRef<int> onMouseButtonDownCallback;
	static bool clickedInViewport;

private:
	static EventNoArg onDrawAdditionalViewportWidgets;
};

constexpr auto InteractiveWidget = UserInterface::UpdateUserInteraction;
