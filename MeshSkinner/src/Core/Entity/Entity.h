#pragma once

#include "Application/Core.h"
#include "EntityComponent.h"
#include "Core/Entity/Transform.h"

class Entity
{
public:
	Entity(const std::string &name = "entity", const Transform &transform = Transform());
	virtual ~Entity() = default;

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
	bool IsWorldMatrixUpdated() const;

protected:
	void RecalculateWorldMatrix();

private:
	// TODO: map of unordered sets as the component storing method - then it's trivial to get all components of type (also more performant)
	//  we would add everything to the set of EntityComponents and then to the set of T if T != EntityComponent and have T default to EntityComponent
	std::unordered_set<Ref<EntityComponent>> components;

public:
	std::string name;
	Transform transform;

private:
	Ref<Entity> parent = nullptr;

	glm::mat4 worldMatrix;
	bool worldMatrixDirty = true;
};
