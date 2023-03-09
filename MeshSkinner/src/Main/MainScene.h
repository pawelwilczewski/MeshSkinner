#pragma once

#include "Application/Core.h"
#include "Application/Scene.h"

#include "MeshSkinner/Tool/Stroke.h"

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
	void OnFileDropped(const std::vector<std::string> &paths);

private:
	Unique<class Brush> brush;
	Unique<Stroke> stroke;
	Unique<class Hierarchy> hierarchy;
	Unique<class AnimationControls> animationControls;
	Unique<class SceneStats> sceneStats;

	Ref<Entity> sceneRoot;

	Ref<class Camera> camera; // TODO: make unique and use weak in renderer? that wont work though? figure it out..
	Ref<class CameraControllerComponent> cameraController;

	std::vector<std::string> unhandledDroppedFiles;

private:
	CallbackRef<StrokeQueryInfo> onStrokeEmplaceCallback;
	CallbackNoArgRef onDrawAdditionalViewportWidgetsCallback;
	CallbackRef<std::vector<std::string>> onFileDroppedCallback;
};
