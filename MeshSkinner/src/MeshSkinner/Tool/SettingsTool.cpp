#include "pch.h"
#include "SettingsTool.h"

#include "Core/Camera/CameraControllerComponent.h"
#include "MeshSkinner/Context.h"

SettingsTool::SettingsTool(const std::string &toolWindowName, CameraControllerComponent *controller) : Tool(toolWindowName), cameraController(controller)
{
}

void SettingsTool::OnUpdateUI()
{
    auto selectedMesh = Context::Get().GetSelectedSkeletalMesh();

    // settings
    ImGui::Begin("Settings");

    InteractiveWidget(ImGui::DragFloat("Mouse sensitivity", &cameraController->mouseSensitivity, 0.0001f, 0.0f, 10.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    if (InteractiveWidget(ImGui::DragFloat("Bone radius", &Context::Get().boneRadius, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)) && selectedMesh)
        Renderer::UpdateBoneRadius(selectedMesh, Context::Get().boneRadius);

    if (InteractiveWidget(ImGui::DragFloat("Tip bone length", &Context::Get().tipBoneLength, 1.f, 0.f, 10000.f, "%.3f", ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_Logarithmic)) && selectedMesh)
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
                    vertex.position.y = Context::Get().tipBoneLength;

            Renderer::UpdateMeshVertices(boneMesh.get());
        }
    }

    InteractiveWidget(ImGui::SliderFloat("Bone alpha", &Renderer::finalAlpha, 0.f, 1.f, "%.3f", ImGuiSliderFlags_ClampOnInput));

    ImGui::End();
}
