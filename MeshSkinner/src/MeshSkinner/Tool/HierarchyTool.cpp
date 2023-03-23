#include "pch.h"
#include "HierarchyTool.h"

Entity *HierarchyTool::selectedEntity = nullptr;

HierarchyTool::HierarchyTool(const std::string &toolWindowName, Entity *root) : Tool(toolWindowName), root(root)
{

}

void HierarchyTool::DrawTree(Entity *entity)
{
    const auto &children = entity->GetChildren();

    // workout the flag to use for this node
    auto flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (children.size() == 0)                   flag |= ImGuiTreeNodeFlags_Leaf;
    if (selectedEntity == entity)    flag |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(entity->name.c_str(), flag))
    {
        if (ImGui::IsItemActivated())
        {
            auto bone = dynamic_cast<Bone *>(entity);

            // if selected a bone - update selectedBone and select the skeletal mesh entity instead
            if (bone)
            {
                auto p = bone->GetParent();

                while (true)
                {
                    auto components = p->GetComponents<SkeletalMeshComponent>();
                    if (components.size() > 0)
                    {
                        selectedEntity = p; // TODO: this shouldn't do this (but we would like to expand the bone's panel still - yet remain in the painting mode)

                        auto &bones = (*components.begin())->skeleton->GetBones();
                        Renderer::selectedBone = std::find(bones.begin(), bones.end(), entity) - bones.begin();
                        break;
                    }

                    p = p->GetParent();
                }
            }
            else
            {
                selectedEntity = entity;
            }
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

void HierarchyTool::OnUpdateUI()
{
    // hierarchy
    ImGui::Begin("Hierarchy");
    DrawTree(root);
    ImGui::End();

    ImGui::Begin("Entity");
    if (selectedEntity)
    {
        ImGui::Text(selectedEntity->name.c_str());
        ImGui::Separator();

        selectedEntity->transform.DisplayUI();

        ImGui::Separator();
        ImGui::Text("Components");
        ImGui::Separator();
        for (const auto &component : selectedEntity->GetComponents<EntityComponent>())
        {
            component->DisplayUI();
            ImGui::Separator();
        }
    }
    ImGui::End();
}
