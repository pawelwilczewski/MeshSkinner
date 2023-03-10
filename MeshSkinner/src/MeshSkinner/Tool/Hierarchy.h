#pragma once

#include "Application/Core.h"
#include "Tool.h"

class Hierarchy : public Tool
{
public:
	Hierarchy(const std::string &toolWindowName, Entity *root);
	virtual ~Hierarchy() = default;

public:
	virtual void OnUpdateUI() override;

private:
	void DrawTree(Entity *entity);

public:
	Entity *GetSelectedEntity() const;

public:
	Entity *root;

private:
	Entity *selectedEntity = nullptr;
};
