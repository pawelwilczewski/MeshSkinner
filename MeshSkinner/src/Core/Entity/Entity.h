#pragma once

#include "Application/Core.h"
#include "EntityComponent.h"
#include "Core/Entity/Transform.h"

class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity(const std::string &name = "entity", Transform transform = Transform());
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

	void AddComponent(Ref<EntityComponent> component);
	void RemoveComponent(Ref<EntityComponent> component);

public:
	void SetParent(const Ref<Entity> &parent);
	const Ref<Entity> &GetParent() const;

	const std::unordered_set<Ref<Entity>> &GetChildren() const;

public:
	const glm::mat4 &GetWorldMatrix();
	bool GetIsWorldMatrixUpdated() const;

protected:
	void RecalculateWorldMatrix();

private:
	void DirtyWorldMatrix();

private:
	std::unordered_set<Ref<EntityComponent>> components; // TODO: components need to be unique ptrs

	std::unordered_set<Ref<Entity>> children;

public:
	std::string name;
	// TODO: this transform shouldn't be settable because of events it holds reference to (replace by GetTransform())
	Transform transform;

private:
	Ref<Entity> parent = nullptr;

	glm::mat4 worldMatrix;
	bool isWorldMatrixUpdated = false;

private:
	CallbackNoArgRef onDirtyMatrixCallback;
};
