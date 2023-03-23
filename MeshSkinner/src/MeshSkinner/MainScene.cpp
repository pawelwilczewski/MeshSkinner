#include "pch.h"
#include "MainScene.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Buffer/Buffer.h"
#include "Core/Camera/Camera.h"
#include "Core/Camera/CameraControllerComponent.h"

#include "MeshSkinner/Tool/Brush.h"
#include "MeshSkinner/Tool/Stroke.h"
#include "MeshSkinner/Tool/Hierarchy.h"
#include "MeshSkinner/Tool/AnimationControls.h"
#include "MeshSkinner/Tool/SceneStats.h"
#include "MeshSkinner/Tool/Settings.h"
#include "MeshSkinner/Tool/WeightColorScheme.h"
#include "MeshSkinner/Tool/ImportExport.h"

MainScene::MainScene() : Scene()
{
    // camera setup
    camera = static_cast<Camera *>(CreateEntity(new Camera("Main Camera")));
    cameraController = MakeRef<CameraControllerComponent>();
    camera->AddComponent(cameraController);
    Renderer::activeCamera = camera;

    // tool initialisation
    brush = MakeUnique<Brush>("Brush Parameters");
    stroke = MakeUnique<Stroke>("Stroke Parameters", [&](StrokeQueryInfo &info) {
        auto selectedMesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>();
        if (!selectedMesh) return;

        info.verts = Renderer::GetFinalVertPosData(selectedMesh.get());
        info.hitTarget = MathUtils::RayMeshIntersection(camera->ProjectViewportToWorld(info.viewportPosition), info.verts, selectedMesh->indices, info.position);
        });
    hierarchy = MakeUnique<Hierarchy>("Hierarchy", GetRoot());
    animationControls = MakeUnique<AnimationControls>();
    sceneStats = MakeUnique<SceneStats>("Scene Stats");
    settings = MakeUnique<Settings>("Settings", cameraController.get());
    weightColorScheme = MakeUnique<WeightColorScheme>("Weight Color Scheme");
    importExport = MakeUnique<ImportExport>("Import Export", this);

    // events
    onDrawAdditionalViewportWidgetsCallback = MakeCallbackNoArgRef([&]() {
        if (Window::GetCursorVisibility())
        {
            auto mousePos = Input::GetMouseScreenPosition();
            ImGui::GetWindowDrawList()->AddCircle(ImVec2(mousePos.x, mousePos.y), brushCircleSize, ImColor(0.8f, 0.8f, 0.8f, 1.f));
        }
        });
    UserInterface::OnDrawAdditionalViewportWidgetsSubscribe(onDrawAdditionalViewportWidgetsCallback);

    onStrokeEmplaceCallback = MakeCallbackRef<StrokeQueryInfo>([&](const StrokeQueryInfo &info) { OnStrokeEmplace(info); });
    stroke->OnStrokeEmplaceSubscribe(onStrokeEmplaceCallback);

    onMouseButtonPressedCallback = MakeCallbackRef<int>([&](int button) { OnMouseButtonPressed(button); });
    Input::OnMouseButtonPressedSubscribe(onMouseButtonPressedCallback);
}

MainScene::~MainScene()
{
    UserInterface::OnDrawAdditionalViewportWidgetsUnsubscribe(onDrawAdditionalViewportWidgetsCallback);
    stroke->OnStrokeEmplaceUnsubscribe(onStrokeEmplaceCallback);

    Input::OnMouseButtonPressedUnsubscribe(onMouseButtonPressedCallback);
}

void MainScene::OnStart()
{
    // TODO: NOW: instead, have a sceneRoot entity and submit it in the renderer (recursively)
    //  also we should be able to remove currently submitted entities
    //  we may need to be able to update entities submitted in renderer
}

void MainScene::OnEarlyUpdate()
{

}

void MainScene::OnUpdate()
{
    auto mesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>().get();

    if (mesh)
    {
        glm::vec3 intersect;
        if (MathUtils::RayMeshIntersection(camera->ProjectViewportToWorld(Input::GetMouseViewportPosition()), mesh, intersect))
        {
            auto offset = camera->transform.GetUpVector() * brush->radius;
            auto screenPos = camera->DeprojectWorldToViewport(intersect + offset);
            auto mousePos = Input::GetMouseViewportPosition();
            brushCircleSize = glm::distance(mousePos, screenPos);
        }
    }
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
    auto selectedMesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>();
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
            // the bone is one of the 
            if (v.bones[i] == Renderer::selectedBone)
            {
                toUpdate = &v.weights[i];
                updated = true;
                break;
            }
        }

        // no such existing weight yet - replace the smallest influence
        //  bone with our active one
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
            v.bones[minWeightBone] = Renderer::selectedBone;
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

    Renderer::UpdateMeshVertices(selectedMesh.get());
}

void MainScene::OnMouseButtonPressed(int button)
{
    // bone select
    if (Input::IsKeyPressed(KEY_LEFT_CONTROL))
    {
        auto selectedMesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>();
        auto ray = camera->ProjectViewportToWorld(Input::GetMouseViewportPosition());

        int i = 0;
        for (const auto &bone : selectedMesh->skeleton->GetBones())
        {
            auto boneMesh = (*bone->GetComponents<StaticMeshComponent>().begin()).get();

            auto verts = Renderer::GetFinalVertPosData(boneMesh);

            glm::vec3 pos;
            if (MathUtils::RayMeshIntersection(ray, verts, boneMesh->indices, pos))
                Renderer::selectedBone = i;

            i++;
        }
    }
}
