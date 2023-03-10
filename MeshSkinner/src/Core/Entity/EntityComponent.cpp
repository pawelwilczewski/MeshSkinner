#include "pch.h"
#include "EntityComponent.h"
#include "Entity.h"

EntityComponent::EntityComponent(const std::string &name) : name(name)
{
}

Entity *EntityComponent::GetEntity() const
{
    return entity;
}

void EntityComponent::OnAttached()
{

}

void EntityComponent::OnDetached()
{

}

void EntityComponent::DisplayUI()
{
    ImGui::Text(name.c_str());
}
