#pragma once

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
};
