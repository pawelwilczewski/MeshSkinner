#include "pch.h"
#include "Entity.h"

Entity::Entity(const std::string &name, const Transform &transform) : name(name), transform(transform)
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

void Entity::SetParent(const Ref<Entity> &parent)
{
	worldMatrixDirty = true;
	this->parent = parent;
}

const Ref<Entity> &Entity::GetParent() const
{
	return this->parent;
}

const glm::mat4 &Entity::GetWorldMatrix()
{
	if (!IsWorldMatrixUpdated())
		RecalculateWorldMatrix();

	return worldMatrix;
}

bool Entity::IsWorldMatrixUpdated() const
{
	if (!worldMatrixDirty)
		return false;

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

	worldMatrixDirty = false;
}
