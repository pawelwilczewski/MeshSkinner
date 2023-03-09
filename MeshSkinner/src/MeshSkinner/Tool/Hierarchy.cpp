#include "pch.h"
#include "Hierarchy.h"

Hierarchy::Hierarchy(const std::string &toolWindowName, const Weak<Entity> &rootEntity) : Tool(toolWindowName), root(rootEntity)
{

}

void Hierarchy::DrawTree(const Ref<Entity> &entity)
{
    const auto &children = entity->GetChildren();

    // workout the flag to use for this node
    auto flag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
    if (children.size() == 0)                   flag |= ImGuiTreeNodeFlags_Leaf;
    if (selectedEntity.lock() == entity)    flag |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::TreeNodeEx(entity->name.c_str(), flag))
    {
        if (ImGui::IsItemActivated())
            selectedEntity = entity;

        for (const auto &child : children)
            DrawTree(child);
        ImGui::TreePop();
    }
}

Ref<Entity> Hierarchy::GetSelectedEntity() const
{
    return selectedEntity.lock();
}

void Hierarchy::OnUpdateUI()
{
    auto selected = selectedEntity.lock();

    // hierarchy
    ImGui::Begin("Hierarchy");
    DrawTree(root.lock());
    ImGui::End();

    ImGui::Begin("Entity");
    if (selected)
    {
        ImGui::Text(selected->name.c_str());
        ImGui::Separator();

        glm::vec3 positionCopy = selected->transform.GetPosition();
        glm::vec3 rotationCopy = selected->transform.GetRotation();
        glm::vec3 scaleCopy = selected->transform.GetScale();
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", glm::value_ptr(positionCopy), 1.f, -1000000000.f, 1000000000.f);
        ImGui::DragFloat3("Rotation", glm::value_ptr(rotationCopy), 1.f, -1000000000.f, 1000000000.f);
        ImGui::DragFloat3("Scale", glm::value_ptr(scaleCopy), 0.05f, -1000000000.f, 1000000000.f);
        selected->transform.SetPosition(positionCopy);
        selected->transform.SetRotation(rotationCopy);
        selected->transform.SetScale(scaleCopy);

        ImGui::Separator();
        ImGui::Text("Components");
        ImGui::Separator();
        for (const auto &component : selected->GetComponents<EntityComponent>())
        {
            ImGui::Text("\tSome component");
            ImGui::Separator();
        }
    }
    ImGui::End();

    // TODO: entity details (transform widget, component names); maybe add some ui function which can be customised in components and will be called for each component
}
