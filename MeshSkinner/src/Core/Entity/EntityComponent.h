#pragma once

#include "Application/Core.h"

class Entity;

class EntityComponent
{
	friend Entity;

public:
	EntityComponent(const std::string &name);
	virtual ~EntityComponent() = default;

public:
	Entity *GetEntity() const;

protected:
	virtual void OnAttached();
	virtual void OnDetached();

public: // TODO: perhaps these DisplayUI functions shouldn't be public
	virtual void DisplayUI();

public:
	std::string name;

private:
	Entity *entity = nullptr;
};
