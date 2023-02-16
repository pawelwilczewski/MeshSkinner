#include "pch.h"
#include "Entity.h"

Entity::Entity(const Transform &transform) : transform(transform)
{
	
}

void Entity::AddComponent(Ref<EntityComponent> component)
{
	components.insert(component);
}

void Entity::RemoveComponent(Ref<EntityComponent> component)
{
	components.erase(component);
}
