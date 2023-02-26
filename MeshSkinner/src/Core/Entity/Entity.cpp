#include "pch.h"
#include "Entity.h"

Entity::Entity(const std::string &name, Transform transform) : name(name), transform(transform)
{
	RecalculateWorldMatrix();

	// setup the dirty matrxi callback
	onDirtyMatrixCallback = MakeCallbackNoArgRef([&]() { DirtyWorldMatrix(); });
	this->transform.OnMatrixDirtySubscribe(onDirtyMatrixCallback);
}

Entity::~Entity()
{
	for (const auto &child : children)
		child->parent = nullptr;

	// cleanup dirty matrix callback
	transform.OnMatrixDirtyUnsubscribe(onDirtyMatrixCallback);
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
	// remove this from parent's children
	if (this->parent)
		this->parent->children.erase(Ref<Entity>(this));

	// update the parent, dirty world matrix
	this->parent = parent;

	// add to new parent's children
	if (this->parent)
		parent->children.insert(Ref<Entity>(this));

	// dirty world matrix for this entity
	DirtyWorldMatrix();
}

const Ref<Entity> &Entity::GetParent() const
{
	return this->parent;
}

const glm::mat4 &Entity::GetWorldMatrix()
{
	if (!isWorldMatrixUpdated)
		RecalculateWorldMatrix();

	return worldMatrix;
}

bool Entity::GetIsWorldMatrixUpdated() const
{
	return isWorldMatrixUpdated;
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

	isWorldMatrixUpdated = true;
}

void Entity::DirtyWorldMatrix()
{
	isWorldMatrixUpdated = false;

	// all children have dirty matrix recursively as well
	for (const auto &child : children)
		child->DirtyWorldMatrix();
}
