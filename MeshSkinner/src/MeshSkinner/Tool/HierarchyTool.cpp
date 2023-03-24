#include "pch.h"
#include "HierarchyTool.h"

HierarchyTool::HierarchyTool(const std::string &toolWindowName, Entity *root) : Tool(toolWindowName), root(root)
{

}

void HierarchyTool::DrawTree(Entity *entity)
{
    const auto &children = entity->GetChildren();

    // workout the flag to use for this node
    auto flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (children.size() == 0)       flag |= ImGuiTreeNodeFlags_Leaf;
    if (selectedBone == entity || (selectedBone == nullptr && selectedEntity == entity))   flag |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(entity->name.c_str(), flag))
    {
        if (ImGui::IsItemActivated())
        {
            auto bone = dynamic_cast<Bone *>(entity);

            // if selected a bone - update selectedBone and select the skeletal mesh entity instead
            if (bone)
            {
                selectedBone = bone;
                auto p = bone->GetParent();

                while (true)
                {
                    auto components = p->GetComponents<SkeletalMeshComponent>();
                    if (components.size() > 0)
                    {
                        selectedEntity = p;
                        selectedSkeletalMesh = (*components.begin()).get();
                        break;
                    }

                    p = p->GetParent();
                }
            }
            else if (entity)
            {
                selectedEntity = entity;
                auto components = selectedEntity->GetComponents<SkeletalMeshComponent>();
                selectedSkeletalMesh = components.size() > 0 ? (*components.begin()).get() : nullptr;
                selectedBone = nullptr;
            }
            else
            {
                selectedEntity = nullptr;
                selectedBone = nullptr;
                selectedSkeletalMesh = nullptr;
            }

            Renderer::selectedEntity = selectedEntity;
            if (selectedEntity && selectedBone)
                Renderer::selectedBone = GetSelectedBoneIndex();
            else
                Renderer::selectedBone = -1;
        }

        for (const auto &child : children)
            DrawTree(child);
        ImGui::TreePop();
    }
}

Entity *HierarchyTool::GetSelectedEntity()
{
    return selectedEntity;
}

Bone *HierarchyTool::GetSelectedBone()
{
    return selectedBone;
}

SkeletalMeshComponent *HierarchyTool::GetSelectedSkeletalMesh()
{
    return selectedSkeletalMesh;
}

uint32_t HierarchyTool::GetSelectedBoneIndex()
{
    auto &bones = selectedSkeletalMesh->skeleton->GetBones();
    return std::find(bones.begin(), bones.end(), HierarchyTool::GetSelectedBone()) - bones.begin();
}

void HierarchyTool::UpdateSelectedBone(uint32_t boneIndex)
{
    selectedBone = selectedSkeletalMesh->skeleton->GetBones()[boneIndex];
    Renderer::selectedBone = GetSelectedBoneIndex();
}

void HierarchyTool::OnUpdateUI()
{
    // hierarchy
    ImGui::Begin("Hierarchy");
    DrawTree(root);
    ImGui::End();

    ImGui::Begin("Entity");
    Entity *entity = selectedBone ? selectedBone : selectedEntity;
    if (entity)
    {
        ImGui::Text(entity->name.c_str());
        ImGui::Separator();

        entity->transform.DisplayUI();

        ImGui::Separator();
        ImGui::Text("Components");
        ImGui::Separator();
        for (const auto &component : entity->GetComponents<EntityComponent>())
        {
            component->DisplayUI();
            ImGui::Separator();
        }
    }
    ImGui::End();
}
