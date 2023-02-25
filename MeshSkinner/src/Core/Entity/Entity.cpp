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
	isWorldMatrixUpdated = false;
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
	if (!isWorldMatrixUpdated)
		return false;

	// no recursion because it's just unnecessary
	auto e = this;
	while (e != nullptr)
	{
		if (!e->transform.IsMatrixUpdated()) 
			return false;

		e = e->parent.get();
	}

	return true;
}

void Entity::RecalculateWorldMatrix()
{
	std::string calc = name;
	worldMatrix = transform.GetMatrix();

	// no recursion because it's just unnecessary
	Ref<Entity> p = this->parent;
	while (p != nullptr)
	{
		worldMatrix = p->transform.GetMatrix() * worldMatrix;
		calc = p->name + " * " + calc;

		p = p->parent;
	}

	isWorldMatrixUpdated = true;
	Log::Info("Calculating transform for {}: {}", name, calc);
}
