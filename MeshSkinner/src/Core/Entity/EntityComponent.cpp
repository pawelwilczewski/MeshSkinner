#include "pch.h"
#include "EntityComponent.h"
#include "Entity.h"

Weak<Entity> EntityComponent::GetEntity() const
{
    return entity;
}
