#include "pch.h"
#include "ImportExportTool.h"

#include "HierarchyTool.h"
#include "SettingsTool.h"

ImportExportTool::ImportExportTool(const std::string &toolWindowName, Scene *scene) : Tool(toolWindowName), scene(scene)
{
    // materials
    ShaderLibrary::Load("Bone", "assets/shaders/Bone.vert", "assets/shaders/Bone.frag", 1);
    ShaderLibrary::Load("WeightPaint", "assets/shaders/WeightPaint.vert", "assets/shaders/WeightPaint.frag", 0);

    boneMaterial = MakeRef<Material>(ShaderLibrary::Get("Bone"));
    weightPaintMaterial = MakeRef<Material>(ShaderLibrary::Get("WeightPaint"));
}

void ImportExportTool::OnUpdateUI()
{
    auto selectedMesh = HierarchyTool::GetSelectedSkeletalMesh();

    ImGui::Begin("Import Export");

    InteractiveWidget(ImGui::InputText("Input file path", &sourceFile));

    auto dropped = Input::GetDroppedFiles();
    if (ImGui::IsItemHovered() && dropped && dropped->size() > 0)
        sourceFile = dropped->at(0);

    if (InteractiveWidget(ImGui::Button("Import as static")))
    {
        Log::Info("Importing static mesh from file {}", sourceFile);

        auto entity = scene->CreateEntity(new Entity());
        entity->AddComponent(MeshLibrary::Import(sourceFile));
        entity->name = std::filesystem::path(sourceFile).filename().string();

        Renderer::Submit(entity);

        Log::Info("Importing static mesh finished");
    }

    if (InteractiveWidget(ImGui::Button("Import as skeletal")))
    {
        Log::Info("Importing skeletal mesh from file {}", sourceFile);

        auto entity = scene->CreateEntity(new Entity());

        auto mesh = MeshLibrary::Import(sourceFile, scene, entity);
        mesh->material = weightPaintMaterial;

        entity->AddComponent(mesh);
        entity->name = std::filesystem::path(sourceFile).filename().string();

        // add bone meshes
        for (auto &bone : mesh->skeleton->GetBones())
        {
            Renderer::Submit(bone);

            // calculate the bone length with some default for tip bones
            auto boneLength = SettingsTool::tipBoneLength;
            auto &children = bone->GetChildren();
            if (children.size() == 1)
                boneLength = glm::length((*bone->GetChildren().begin())->transform.GetPosition());

            auto boneMesh = MeshLibrary::GetBone(boneLength);
            boneMesh->material = boneMaterial;
            bone->AddComponent(boneMesh);
        }

        Renderer::Submit(entity);
        Renderer::UpdateBoneRadius(mesh.get(), SettingsTool::boneRadius);

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
}
