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

    // materials
    ShaderLibrary::Load("Bone", "assets/shaders/Bone.vert", "assets/shaders/Bone.frag", 1);
    ShaderLibrary::Load("WeightPaint", "assets/shaders/WeightPaint.vert", "assets/shaders/WeightPaint.frag", 0);

    weightPaintMaterial = MakeRef<Material>(ShaderLibrary::Get("WeightPaint"));

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
    // TODO: all of these can be Tools - make that happen

    auto selectedMesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>();

    ImGui::Begin("Import Export");

    InteractiveWidget(ImGui::InputText("Input file path", &sourceFile));

    auto dropped = Input::GetDroppedFiles();
    if (ImGui::IsItemHovered() && dropped && dropped->size() > 0)
        sourceFile = dropped->at(0);

    if (InteractiveWidget(ImGui::Button("Import as static")))
    {
        Log::Info("Importing static mesh from file {}", sourceFile);

        auto entity = CreateEntity(new Entity());
        entity->AddComponent(MeshLibrary::Import(sourceFile));
        entity->name = std::filesystem::path(sourceFile).filename().string();

        Renderer::Submit(entity);

        Log::Info("Importing static mesh finished");
    }

    if (InteractiveWidget(ImGui::Button("Import as skeletal")))
    {
        Log::Info("Importing skeletal mesh from file {}", sourceFile);

        auto entity = CreateEntity(new Entity());

        auto mesh = MeshLibrary::Import(sourceFile, this, entity);
        mesh->material = weightPaintMaterial;

        entity->AddComponent(mesh);
        entity->name = std::filesystem::path(sourceFile).filename().string();

        // add bone meshes
        auto boneMat = MakeRef<Material>(ShaderLibrary::Get("Bone"));
        for (auto &bone : mesh->skeleton->GetBones())
        {
            Renderer::Submit(bone);

            // calculate the bone length with some default for tip bones
            auto boneLength = Settings::tipBoneLength;
            auto &children = bone->GetChildren();
            if (children.size() == 1)
                boneLength = glm::length((*bone->GetChildren().begin())->transform.GetPosition());

            auto boneMesh = MeshLibrary::GetBone(boneLength);
            boneMesh->material = boneMat;
            bone->AddComponent(boneMesh);
        }

        Renderer::UpdateBoneRadius(mesh.get());

        Renderer::Submit(entity);

        Log::Info("Importing skeletal mesh finished");
    }

    InteractiveWidget(ImGui::InputText("Export file path", &targetFile));
    if (ImGui::IsItemHovered() && dropped && dropped->size() > 0)
        targetFile = dropped->at(0);

    if (InteractiveWidget(ImGui::Button("Export file")))
    {
        Log::Info("Exporting updated mesh from {} to {}", sourceFile, targetFile);

        MeshLibrary::ExportUpdated(sourceFile, targetFile, selectedMesh);

        Log::Info("Exporting finished");
    }

    ImGui::End();

    ImGui::Begin("Color scheme");
    ImGui::ColorEdit3("Colour   0%", glm::value_ptr(Renderer::color000));
    ImGui::ColorEdit3("Colour  25%", glm::value_ptr(Renderer::color025));
    ImGui::ColorEdit3("Colour  50%", glm::value_ptr(Renderer::color050));
    ImGui::ColorEdit3("Colour  75%", glm::value_ptr(Renderer::color075));
    ImGui::ColorEdit3("Colour 100%", glm::value_ptr(Renderer::color100));
    ImGui::End();
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
