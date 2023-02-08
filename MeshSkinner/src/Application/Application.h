#pragma once

#include "Core.h"
#include "Event.h"

class Application
{
public:
	static void Init(uint32_t width, uint32_t height, const char *title, int vsync);
	static void Run();
	static void Terminate();

private:
	static void Start();
	static void EarlyUpdate();
	static void Update();
	static void UpdateUI();
	static void LateUpdate();
	static void End();

public:
	static void OnStartSubscribe(CallbackNoArgRef callback);
	static void OnEarlyUpdateSubscribe(CallbackNoArgRef callback);
	static void OnUpdateSubscribe(CallbackNoArgRef callback);
	static void OnUpdateUISubscribe(CallbackNoArgRef callback);
	static void OnLateUpdateSubscribe(CallbackNoArgRef callback);
	static void OnEndSubscribe(CallbackNoArgRef callback);

	static void OnStartUnsubscribe(CallbackNoArgRef callback);
	static void OnEarlyUpdateUnsubscribe(CallbackNoArgRef callback);
	static void OnUpdateUnsubscribe(CallbackNoArgRef callback);
	static void OnUpdateUIUnsubscribe(CallbackNoArgRef callback);
	static void OnLateUpdateUnsubscribe(CallbackNoArgRef callback);
	static void OnEndUnsubscribe(CallbackNoArgRef callback);

public:
	static Unique<class Scene> scene;

private:
	static EventNoArg onStart;
	static EventNoArg onEarlyUpdate;
	static EventNoArg onUpdate;
	static EventNoArg onUpdateUI;
	static EventNoArg onLateUpdate;
	static EventNoArg onEnd;
};
