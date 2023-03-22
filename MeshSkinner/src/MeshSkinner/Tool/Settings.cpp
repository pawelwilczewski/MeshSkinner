#include "pch.h"
#include "Settings.h"

#include "Hierarchy.h"
#include "Core/Camera/CameraControllerComponent.h"

Settings::Settings(const std::string &toolWindowName, CameraControllerComponent *controller) : Tool(toolWindowName), cameraController(controller)
{
}

void Settings::OnUpdateUI()
{
    auto selectedMesh = Hierarchy::GetSelectedComponent<SkeletalMeshComponent>();

    // settings
    ImGui::Begin("Settings");

    InteractiveWidget(ImGui::DragFloat("Mouse sensitivity", &cameraController->mouseSensitivity, 0.0001f, 0.0f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    if (InteractiveWidget(ImGui::DragFloat("Bone radius", &boneRadius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)) && selectedMesh)
        Renderer::UpdateBoneRadius(selectedMesh.get());

    if (InteractiveWidget(ImGui::DragFloat("Tip bone length", &tipBoneLength, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)) && selectedMesh)
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
}
