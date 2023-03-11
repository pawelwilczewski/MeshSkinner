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

private:
	Unique<class Brush> brush;
	Unique<Stroke> stroke;
	Unique<class Hierarchy> hierarchy;
	Unique<class AnimationControls> animationControls;
	Unique<class SceneStats> sceneStats;

	Camera *camera;
	Ref<class CameraControllerComponent> cameraController;

	std::string sourceFile;
	std::string targetFile;

	Ref<Material> weightPaintMaterial;

private:
	CallbackRef<StrokeQueryInfo> onStrokeEmplaceCallback;
	CallbackNoArgRef onDrawAdditionalViewportWidgetsCallback;
};
