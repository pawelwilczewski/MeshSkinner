#pragma once

#include "Core/Entity/Entity.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

protected:
	virtual void OnStart() = 0;
	virtual void OnEarlyUpdate() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnUpdateUI() = 0;
	virtual void OnLateUpdate() = 0;
	virtual void OnEnd() = 0;

public:
	Entity *CreateEntity(Entity *entity);
	void DestroyEntity(const Entity *entity);

	Entity *GetRoot() const;

protected:
	std::vector<Unique<Entity>> entities;

private:
	CallbackNoArgRef onStartCallback;
	CallbackNoArgRef onEarlyUpdateCallback;
	CallbackNoArgRef onUpdateCallback;
	CallbackNoArgRef onUpdateUICallback;
	CallbackNoArgRef onLateUpdateCallback;
	CallbackNoArgRef onEndCallback;
};
