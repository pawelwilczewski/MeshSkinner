#pragma once

#include "Application/Core.h"

class EntityComponent
{
	friend class Entity;

public:
	EntityComponent() = default;
	virtual ~EntityComponent() = default;

public:
	Weak<Entity> GetEntity() const;

private:
	Weak<Entity> entity;
};
