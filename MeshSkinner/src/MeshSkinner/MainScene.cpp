#include "pch.h"
#include "MainScene.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Buffer/Buffer.h"
#include "Core/Camera/Camera.h"
#include "Core/Camera/CameraControllerComponent.h"

#include "MeshSkinner/Tool/BrushTool.h"
#include "MeshSkinner/Tool/StrokeTool.h"
#include "MeshSkinner/Tool/HierarchyTool.h"
#include "MeshSkinner/Tool/AnimationControlsTool.h"
#include "MeshSkinner/Tool/SceneStatsTool.h"
#include "MeshSkinner/Tool/SettingsTool.h"
#include "MeshSkinner/Tool/WeightColorSchemeTool.h"
#include "MeshSkinner/Tool/ImportExportTool.h"

MainScene::MainScene() : Scene()
{
    // camera setup
    camera = static_cast<Camera *>(CreateEntity(new Camera("Main Camera")));
    cameraController = MakeRef<CameraControllerComponent>();
    camera->AddComponent(cameraController);
    Renderer::activeCamera = camera;

    // tool initialisation
    brush = MakeUnique<BrushTool>("Brush Parameters");
    brush->camera = camera;
    stroke = MakeUnique<StrokeTool>("Stroke Parameters", [&](StrokeQueryInfo &info) {
        auto selectedMesh = HierarchyTool::GetSelectedSkeletalMesh();
        if (!selectedMesh) return;

        info.verts = Renderer::GetFinalVertPosData(selectedMesh);
        info.hitTarget = MathUtils::RayMeshIntersection(camera->ProjectViewportToWorld(info.viewportPosition), info.verts, selectedMesh->indices, info.position);
        });
    hierarchy = MakeUnique<HierarchyTool>("Hierarchy", GetRoot());
    animationControls = MakeUnique<AnimationControlsTool>();
    sceneStats = MakeUnique<SceneStatsTool>("Scene Stats");
    settings = MakeUnique<SettingsTool>("Settings", cameraController.get());
    weightColorScheme = MakeUnique<WeightColorSchemeTool>("Weight Color Scheme");
    importExport = MakeUnique<ImportExportTool>("Import Export", this);

    // events
    onStrokeEmplaceCallback = MakeCallbackRef<StrokeQueryInfo>([&](const StrokeQueryInfo &info) { OnStrokeEmplace(info); });
    stroke->OnStrokeEmplaceSubscribe(onStrokeEmplaceCallback);

    onMouseButtonPressedCallback = MakeCallbackRef<int>([&](int button) { OnMouseButtonPressed(button); });
    Input::OnMouseButtonPressedSubscribe(onMouseButtonPressedCallback);
}

MainScene::~MainScene()
{
    stroke->OnStrokeEmplaceUnsubscribe(onStrokeEmplaceCallback);
    Input::OnMouseButtonPressedUnsubscribe(onMouseButtonPressedCallback);
}

void MainScene::OnStart()
{

}

void MainScene::OnEarlyUpdate()
{

}

void MainScene::OnUpdate()
{

}

void MainScene::OnUpdateUI()
{

}

void MainScene::OnLateUpdate()
{
    
}

void MainScene::OnEnd()
{

}

void MainScene::OnStrokeEmplace(const StrokeQueryInfo &info)
{
    // weight painting logic

    auto selectedMesh = HierarchyTool::GetSelectedSkeletalMesh();
    if (!selectedMesh) return;

    auto vertIndices = MathUtils::GetVerticesInRadius(info.verts, info.position, brush->radius);

    for (const auto &vIndex : vertIndices)
    {
        auto &v = selectedMesh->vertices[vIndex];

        // try to update an already existing weight
        float *toUpdate;
        bool updated = false;
        for (size_t i = 0; i < v.bones.length(); i++)
        {
            // the bone already exists in vertex info
            if (v.bones[i] == HierarchyTool::GetSelectedBoneIndex())
            {
                toUpdate = &v.weights[i];
                updated = true;
                break;
            }
        }

        // no such existing weight yet - replace the smallest influence bone with our active one
        if (!updated)
        {
            auto minWeightBone = 0;
            auto minWeight = v.weights[0];

            for (int i = v.weights.length() - 1; i > 0; i--)
            {
                if (v.weights[i] < minWeight)
                {
                    minWeight = v.weights[i];
                    minWeightBone = i;
                }
            }

            // update the weights
            v.bones[minWeightBone] = HierarchyTool::GetSelectedBoneIndex();
            v.weights[minWeightBone] = 0.f;
            toUpdate = &v.weights[minWeightBone];
        }

        // update the weight
        (*toUpdate) = brush->Blend(*toUpdate, glm::distance(info.position, glm::vec3(info.verts[vIndex])));

        // the components of the result must add up to one
        auto sum = 0.f;
        for (int i = 0; i < v.weights.length(); i++)
            sum += v.weights[i];
        v.weights /= sum;
    }

    Renderer::UpdateMeshVertices(selectedMesh);
}

void MainScene::OnMouseButtonPressed(int button)
{
    // bone selection
    if (Input::IsKeyPressed(KEY_LEFT_CONTROL))
    {
        auto selectedMesh = HierarchyTool::GetSelectedSkeletalMesh();
        auto ray = camera->ProjectViewportToWorld(Input::GetMouseViewportPosition());

        int i = 0;
        for (const auto &bone : selectedMesh->skeleton->GetBones())
        {
            auto boneMesh = (*bone->GetComponents<StaticMeshComponent>().begin()).get();

            auto verts = Renderer::GetFinalVertPosData(boneMesh);

            glm::vec3 pos;
            if (MathUtils::RayMeshIntersection(ray, verts, boneMesh->indices, pos))
                HierarchyTool::UpdateSelectedBone(i);

            i++;
        }
    }
}
