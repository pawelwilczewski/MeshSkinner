#pragma once

#include "Application/Core.h"
#include "EntityComponent.h"
#include "Core/Entity/Transform.h"

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity(const std::string &name = "entity", const Transform &transform = Transform());
	virtual ~Entity();

public:
	template<typename T>
	const std::unordered_set<Ref<T>> GetComponents() const
	{
		auto result = std::unordered_set<Ref<T>>();
		for (const auto &component : components)
		{
			auto asT = std::dynamic_pointer_cast<T>(component);
			if (asT) result.insert(asT);
		}

		return result;
	}

	Ref<EntityComponent> AddComponent(const Ref<EntityComponent> &component);
	void RemoveComponent(const Ref<EntityComponent> &component);

public:
	void SetParent(Entity *parent);
	Entity *GetParent() const;

	const std::unordered_set<Entity *> &GetChildren() const;

public:
	const glm::mat4 &GetWorldMatrix();
	bool GetIsWorldMatrixUpdated() const;

protected:
	void RecalculateWorldMatrix();

private:
	void DirtyWorldMatrix();

private:
	std::unordered_set<Ref<EntityComponent>> components;
	std::unordered_set<Entity *> children;

public:
	std::string name;
	// TODO: this transform shouldn't be settable because of events it holds reference to (replace by Transform const &GetTransform())
	Transform transform;

private:
	Entity *parent = nullptr;

	glm::mat4 worldMatrix;
	bool isWorldMatrixUpdated = false;

private:
	CallbackNoArgRef onDirtyMatrixCallback;
};
