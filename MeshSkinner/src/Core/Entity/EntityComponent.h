#pragma once

#include "Application/Core.h"

class EntityComponent
{
	friend class Entity;

public:
	EntityComponent(const std::string &name);
	virtual ~EntityComponent() = default;

public:
	Weak<Entity> GetEntity() const;

protected:
	virtual void OnAttached() {}
	virtual void OnDetached() {}

public:
	std::string name;

private:
	Weak<Entity> entity;
};
