#include "pch.h"
#include "Hierarchy.h"

Entity *Hierarchy::selectedEntity = nullptr;

Hierarchy::Hierarchy(const std::string &toolWindowName, Entity *root) : Tool(toolWindowName), root(root)
{

}

void Hierarchy::DrawTree(Entity *entity)
{
    const auto &children = entity->GetChildren();

    // workout the flag to use for this node
    auto flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (children.size() == 0)                   flag |= ImGuiTreeNodeFlags_Leaf;
    if (selectedEntity == entity)    flag |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(entity->name.c_str(), flag))
    {
        if (ImGui::IsItemActivated())
            selectedEntity = entity;

        for (const auto &child : children)
            DrawTree(child);
        ImGui::TreePop();
    }
}

Entity *Hierarchy::GetSelectedEntity()
{
    return selectedEntity;
}

void Hierarchy::OnUpdateUI()
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
