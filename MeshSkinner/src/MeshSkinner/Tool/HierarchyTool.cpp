#include "pch.h"
#include "HierarchyTool.h"

#include "MeshSkinner/Context.h"

HierarchyTool::HierarchyTool(const std::string &toolWindowName, Entity *root) : Tool(toolWindowName), root(root)
{

}

void HierarchyTool::DrawTree(Entity *entity)
{
    const auto &children = entity->GetChildren();

    // workout the flag to use for this node
    auto flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (children.size() == 0)       flag |= ImGuiTreeNodeFlags_Leaf;
    if (Context::Get().GetSelectedBone() == entity || (Context::Get().GetSelectedBone() == nullptr && Context::Get().GetSelectedEntity() == entity))   flag |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(entity, flag, entity->name.c_str()))
    {
        if (ImGui::IsItemActivated())
            Context::Get().UpdateSelection(entity);

        for (const auto &child : children)
            DrawTree(child);
        ImGui::TreePop();
    }
}

void HierarchyTool::OnUpdateUI()
{
    // hierarchy
    ImGui::Begin("Hierarchy");
    DrawTree(root);
    ImGui::End();

    ImGui::Begin("Entity");
    auto bone = Context::Get().GetSelectedBone();
    Entity *entity = bone ? bone : Context::Get().GetSelectedEntity();
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
