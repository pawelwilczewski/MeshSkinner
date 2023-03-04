#pragma once

#include "Application/Core.h"
#include "Application/Scene.h"

#include "MeshSkinner/Stroke.h"

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

private:
	void OnStrokeEmplace(const StrokeQueryInfo &info);

private:
	Unique<class Brush> brush;
	Unique<class Stroke> stroke;

	Ref<class Camera> camera; // TODO: make unique and use weak in renderer? that wont work though? figure it out..
	Unique<class CameraController> cameraController;

private:
	CallbackRef<StrokeQueryInfo> onStrokeQueryCallback;

	CallbackRef<StrokeQueryInfo> onStrokeEmplaceCallback;
};
