#pragma once

#include "Application/Core.h"
#include "Core/Entity/Scene.h"

struct StrokeQueryInfo;

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

	void OnMouseButtonPressed(int button);

private:
	Camera *camera;
	Ref<class CameraControllerComponent> cameraController;

private:
	Unique<class BrushTool> brush;
	Unique<class StrokeTool> stroke;
	Unique<class HierarchyTool> hierarchy;
	Unique<class AnimationControlsTool> animationControls;
	Unique<class SceneStatsTool> sceneStats;
	Unique<class SettingsTool> settings;
	Unique<class WeightColorSchemeTool> weightColorScheme;
	Unique<class ImportExportTool> importExport;

private:
	CallbackRef<StrokeQueryInfo> onStrokeEmplaceCallback;
	CallbackRef<int> onMouseButtonPressedCallback;
};
