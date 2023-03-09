#include "pch.h"
#include "EntityComponent.h"
#include "Entity.h"

EntityComponent::EntityComponent(const std::string &name) : name(name)
{
}

Weak<Entity> EntityComponent::GetEntity() const
{
    return entity;
}
