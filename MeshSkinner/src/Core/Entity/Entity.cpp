#include "pch.h"
#include "Entity.h"

Entity::Entity(const Transform &transform, const std::string &name) : name(name)
{
	RecalculateWorldMatrix();
}

void Entity::AddComponent(Ref<EntityComponent> component)
{
	components.insert(component);
}

void Entity::RemoveComponent(Ref<EntityComponent> component)
{
	components.erase(component);
}

const glm::mat4 &Entity::GetWorldMatrix()
{
	if (!IsWorldMatrixUpdated())
		RecalculateWorldMatrix();

	return worldMatrix;
}

bool Entity::IsWorldMatrixUpdated() const
{
	auto updated = transform.IsMatrixUpdated();

	if (!updated)
		return false;

	// no recursion because it's just unnecessary
	Ref<Entity> p = this->parent;
	while (p != nullptr)
	{
		if (!p->transform.IsMatrixUpdated()) 
			return false;

		p = p->parent;
	}

	return true;
}

void Entity::RecalculateWorldMatrix()
{
	worldMatrix = transform.GetMatrix();

	// no recursion because it's just unnecessary
	Ref<Entity> p = this->parent;
	while (p != nullptr)
	{
		worldMatrix = p->transform.GetMatrix() * worldMatrix;

		p = p->parent;
	}
}
