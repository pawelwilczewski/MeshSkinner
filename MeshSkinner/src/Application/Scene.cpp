#include "pch.h"
#include "Scene.h"

static CallbackNoArgRef onStartCallback;
static CallbackNoArgRef onEarlyUpdateCallback;
static CallbackNoArgRef onUpdateCallback;
static CallbackNoArgRef onUpdateUICallback;
static CallbackNoArgRef onLateUpdateCallback;
static CallbackNoArgRef onEndCallback;

Scene::Scene()
{
	onStartCallback = MakeCallbackNoArgRef([&]() { OnStart(); });
	onEarlyUpdateCallback = MakeCallbackNoArgRef([&]() { OnEarlyUpdate(); });
	onUpdateCallback = MakeCallbackNoArgRef([&]() { OnUpdate(); });
	onUpdateUICallback = MakeCallbackNoArgRef([&]() { OnUpdateUI(); });
	onLateUpdateCallback = MakeCallbackNoArgRef([&]() { OnLateUpdate(); });
	onEndCallback = MakeCallbackNoArgRef([&]() { OnEnd(); });

	Application::OnStartSubscribe(onStartCallback);
	Application::OnEarlyUpdateSubscribe(onEarlyUpdateCallback);
	Application::OnUpdateSubscribe(onUpdateCallback);
	Application::OnUpdateUISubscribe(onUpdateUICallback);
	Application::OnLateUpdateSubscribe(onLateUpdateCallback);
	Application::OnEndSubscribe(onEndCallback);
}

Scene::~Scene()
{
	Application::OnStartUnsubscribe(onStartCallback);
	Application::OnEarlyUpdateUnsubscribe(onEarlyUpdateCallback);
	Application::OnUpdateUnsubscribe(onUpdateCallback);
	Application::OnUpdateUIUnsubscribe(onUpdateUICallback);
	Application::OnLateUpdateUnsubscribe(onLateUpdateCallback);
	Application::OnEndUnsubscribe(onEndCallback);
}
