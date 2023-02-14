#pragma once

#include "EntityComponent.h"

class Entity
{
public:
	Entity(const Transform &transform = Transform());
	virtual ~Entity() = default;

public:
	Transform transform;

	template<typename T>
	const std::unordered_set<const Ref<T> &> GetComponents() const
	{
		auto result = std::unordered_set<const Ref<T> &>();
		for (const auto &component : components)
			if (dynamic_cast<T *>(component.get()))
				result.insert(component);

		return result;
	}

	void AddComponent(Ref<EntityComponent> component);
	void RemoveComponent(Ref<EntityComponent> component);

private:
	// TODO: map of unordered sets as the component storing method - then it's trivial to get all components of type (also more performant)
	//  we would add everything to the set of EntityComponents and then to the set of T if T != EntityComponent and have T default to EntityComponent
	std::unordered_set<Ref<EntityComponent>> components;
};
