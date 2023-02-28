#pragma once

#include "Application/Core.h"
#include "Application/Scene.h"

class MainScene : public Scene
{
public:
	MainScene();
	virtual ~MainScene();

protected:
	virtual void OnStart() override;
	virtual void OnEarlyUpdate() override;
	virtual void OnUpdate() override;
	virtual void OnUpdateUI() override;
	virtual void OnLateUpdate() override;
	virtual void OnEnd() override;

	void OnMouseButtonPressed(int button);

private:
	CallbackRef<int> onMouseButtonPressedCallback;
};
