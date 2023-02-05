#pragma once

#include "Application/Core.h"
#include "Application/Scene.h"

class MainScene : public Scene
{
public:
	MainScene();
	virtual ~MainScene();

protected:
	virtual void Start() override;
	virtual void EarlyUpdate() override;
	virtual void Update() override;
	virtual void UpdateUI() override;
	virtual void LateUpdate() override;
	virtual void End() override;
};
