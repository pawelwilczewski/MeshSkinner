#pragma once

#include "Application/Core.h"
#include "EntityComponent.h"
#include "Core/Entity/Transform.h"

class Entity
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

public:
	const glm::mat4 &GetWorldMatrix();
	bool GetIsWorldMatrixUpdated() const;

protected:
	void RecalculateWorldMatrix();

private:
	void DirtyWorldMatrix();

private:
	// TODO: map of unordered sets as the component storing method - then it's trivial to get all components of type (also more performant)
	//  we would add everything to the set of EntityComponents and then to the set of T if T != EntityComponent and have T default to EntityComponent
	std::unordered_set<Ref<EntityComponent>> components;

	std::unordered_set<Ref<Entity>> children;

public:
	std::string name;
	Transform transform;

private:
	Ref<Entity> parent = nullptr;

	glm::mat4 worldMatrix;
	bool isWorldMatrixUpdated = false;

private:
	CallbackNoArgRef onDirtyMatrixCallback;
};
