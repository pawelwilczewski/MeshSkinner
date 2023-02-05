#pragma once

class Scene
{
	friend class Application;

public:
	Scene() = default;
	virtual ~Scene() = default;

protected:
	virtual void Start() = 0;
	virtual void EarlyUpdate() = 0;
	virtual void Update() = 0;
	virtual void UpdateUI() = 0;
	virtual void LateUpdate() = 0;
	virtual void End() = 0;
};
