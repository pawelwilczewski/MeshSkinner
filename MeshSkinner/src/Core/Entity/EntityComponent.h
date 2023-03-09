#pragma once

#include "Application/Core.h"

class EntityComponent
{
	friend class Entity;

public:
	EntityComponent(const std::string &name);
	virtual ~EntityComponent() = default;

public:
	Weak<class Entity> GetEntity() const;

protected:
	virtual void OnAttached() {}
	virtual void OnDetached() {}

public: // TODO: perhaps these DisplayUI functions shouldn't be public
	virtual void DisplayUI();

public:
	std::string name;

private:
	Weak<class Entity> entity;
};
