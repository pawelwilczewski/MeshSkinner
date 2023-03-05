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
	// cleanup dirty matrix callback
	transform.OnMatrixDirtyUnsubscribe(onDirtyMatrixCallback);

	// TODO: this is causing read access violation upon exit (obviously)
	// remove self from parent's children
	if (parent)
		parent->children.erase(shared_from_this());

	// reset parent for all children
	for (const auto &child : children)
		child->parent.reset();
}

void Entity::AddComponent(Ref<EntityComponent> component)
{
	components.insert(component);

	component->entity = weak_from_this();
	component->OnAttached();
}

void Entity::RemoveComponent(Ref<EntityComponent> component)
{
	components.erase(component);

	component->entity.reset();
	component->OnDetached();
}

void Entity::SetParent(const Ref<Entity> &parent)
{
	// remove this from parent's children
	if (this->parent)
		this->parent->children.erase(shared_from_this());

	// update the parent, dirty world matrix
	this->parent = parent;

	// add to new parent's children
	if (this->parent)
		parent->children.insert(shared_from_this());

	// dirty world matrix for this entity
	DirtyWorldMatrix();
}

const Ref<Entity> &Entity::GetParent() const
{
	return this->parent;
}

const std::unordered_set<Ref<Entity>> &Entity::GetChildren() const
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
