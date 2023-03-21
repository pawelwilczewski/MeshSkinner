#include "pch.h"
#include "Scene.h"

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

	entities.push_back(MakeUnique<Entity>("root"));
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

Entity *Scene::CreateEntity(Entity *entity) // TODO: consider converting this into some auto template function which would return the exact type so no need for casting if further used
{
	entities.push_back(Unique<Entity>(entity));

	// set the parent to scene root
	entity->SetParent(GetRoot());

	return entity;
}

void Scene::DestroyEntity(const Entity *entity)
{
	for (auto e = entities.begin(); e < entities.end(); e++)
		if ((*e).get() == entity)
			entities.erase(e);
}

Entity *Scene::GetRoot() const
{
	return entities[0].get();
}
