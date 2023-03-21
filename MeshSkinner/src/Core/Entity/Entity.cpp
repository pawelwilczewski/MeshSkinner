#include "pch.h"
#include "Entity.h"

Entity::Entity(const std::string &name, const Transform &transform) : name(name), transform(transform)
{
	RecalculateWorldMatrix();

	// setup the dirty matrix callback
	onDirtyMatrixCallback = MakeCallbackNoArgRef([&]() { DirtyWorldMatrix(); });
	this->transform.OnMatrixDirtySubscribe(onDirtyMatrixCallback);
}

Entity::~Entity()
{
	// cleanup dirty matrix callback
	transform.OnMatrixDirtyUnsubscribe(onDirtyMatrixCallback);

	if (parent)
		parent->children.erase(this);

	// reset parent for all children
	for (const auto &child : children)
		child->parent = nullptr;
}

Ref<EntityComponent> Entity::AddComponent(const Ref<EntityComponent> &component)
{
	components.insert(component);

	component->entity = this;
	component->OnAttached();

	return component;
}

void Entity::RemoveComponent(const Ref<EntityComponent> &component)
{
	component->OnDetached();

	components.erase(component);
}

void Entity::SetParent(Entity *parent)
{
	// remove this from parent's children
	if (this->parent)
		this->parent->children.erase(this);

	// update the parent, dirty world matrix
	this->parent = parent;

	// add to new parent's children
	if (this->parent)
		parent->children.insert(this);

	// dirty world matrix for this entity
	DirtyWorldMatrix();
}

Entity *Entity::GetParent() const
{
	return this->parent;
}

const std::unordered_set<Entity *> &Entity::GetChildren() const
{
	return children;
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

Transform Entity::GetWorldTransform()
{
	assert(false);

	// this doesn't work quite right because the * operator is not correct (scale is troublesome)
	Transform t = transform;

	auto p = GetParent();
	while (p != nullptr)
	{
		t = p->transform * t;
		p = p->GetParent();
	}

	return t;
}

void Entity::RecalculateWorldMatrix()
{
	worldMatrix = transform.GetMatrix();

	// no recursion because it's just unnecessary
	auto p = this->parent;
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
