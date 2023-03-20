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

MainScene::MainScene() : Scene()
{
    camera = static_cast<Camera *>(CreateEntity(new Camera("Main Camera")));

    // camera setup
    cameraController = MakeRef<CameraControllerComponent>("CameraControllerComponent 0", 10.f);
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
    sceneStats = MakeUnique<SceneStats>();

    // callbacks
    onDrawAdditionalViewportWidgetsCallback = MakeCallbackNoArgRef([&]() {
        if (Window::GetCursorVisibility())
        {
            auto mousePos = Input::GetMouseScreenPosition();
            ImGui::GetWindowDrawList()->AddCircle(ImVec2(mousePos.x, mousePos.y), 10.f, ImColor(0.8f, 0.8f, 0.8f, 1.f));
        }
        });
    UserInterface::OnDrawAdditionalViewportWidgetsSubscribe(onDrawAdditionalViewportWidgetsCallback);

    onStrokeEmplaceCallback = MakeCallbackRef<StrokeQueryInfo>([&](const StrokeQueryInfo &info) { OnStrokeEmplace(info); });
    stroke->OnStrokeEmplaceSubscribe(onStrokeEmplaceCallback);

    ShaderLibrary::Load("Bone", "assets/shaders/Bone.vert", "assets/shaders/Bone.frag", 1);
    ShaderLibrary::Load("WeightPaint", "assets/shaders/WeightPaint.vert", "assets/shaders/WeightPaint.frag", 0);

    weightPaintMaterial = MakeRef<Material>(ShaderLibrary::Get("WeightPaint"));

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

}

void MainScene::OnUpdateUI()
{
    // TODO: all of these can be Tools - make that happen

    
    // edited mesh
    ImGui::Begin("Weight Painting");

    auto selectedMesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>();
    if (selectedMesh)
        InteractiveWidget(ImGui::SliderInt("ActiveBone", &Renderer::selectedBone, 0, selectedMesh->skeleton->GetBones().size() - 1));

    ImGui::End();

    // settings
    ImGui::Begin("Settings");

    InteractiveWidget(ImGui::DragFloat("Mouse sensitivity", &cameraController->mouseSensitivity, 0.0001f, 0.0f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    Ref<SkeletalMeshComponent> updateBoneRadiusSkeletalMesh = nullptr;
    static float boneRadius = 5.f;
    if (selectedMesh && InteractiveWidget(ImGui::DragFloat("Bone radius", &boneRadius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)))
        updateBoneRadiusSkeletalMesh = selectedMesh;

    static float tipBoneLength = 50.f;
    if (selectedMesh && InteractiveWidget(ImGui::DragFloat("Tip bone length", &tipBoneLength, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)))
    {
        auto &bones = selectedMesh->skeleton->GetBones();
        for (auto &bone : bones)
        {
            if (bone->GetChildren().size() > 0) continue;

            auto &boneMeshes = bone->GetComponents<StaticMeshComponent>();
            if (boneMeshes.size() == 0) continue;

            auto &boneMesh = *boneMeshes.begin();
            if (!boneMesh) continue;

            for (auto &vertex : boneMesh->vertices)
                if (vertex.position.y > glm::epsilon<float>())
                    vertex.position.y = tipBoneLength;

            Renderer::UpdateMeshVertices(boneMesh.get());
        }
    }

    ImGui::End();

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
            auto boneLength = tipBoneLength;
            auto &children = bone->GetChildren();
            if (children.size() == 1)
                boneLength = glm::length((*bone->GetChildren().begin())->transform.GetPosition());

            auto boneMesh = MeshLibrary::GetBone(boneLength);
            boneMesh->material = boneMat;
            bone->AddComponent(boneMesh);
        }

        updateBoneRadiusSkeletalMesh = mesh;

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

    if (updateBoneRadiusSkeletalMesh)
    {
        auto &bones = updateBoneRadiusSkeletalMesh->skeleton->GetBones();
        for (auto &bone : bones)
        {
            auto &boneMeshes = bone->GetComponents<StaticMeshComponent>();
            if (boneMeshes.size() == 0) continue;

            auto &boneMesh = *boneMeshes.begin();
            if (!boneMesh) continue;

            for (auto &vertex : boneMesh->vertices)
            {
                vertex.position.x = glm::sign(vertex.position.x) * boneRadius;
                vertex.position.z = glm::sign(vertex.position.z) * boneRadius;
            }

            Renderer::UpdateMeshVertices(boneMesh.get());
        }
    }
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
