#pragma once

#include "Application/Core.h"
#include "Tool.h"

class Hierarchy : public Tool
{
public:
	Hierarchy(const std::string &toolWindowName, const Weak<Entity> &rootEntity);
	virtual ~Hierarchy() = default;

public:
	virtual void OnUpdateUI() override;

private:
	void DrawTree(const Ref<Entity> &entity);

public:
	Weak<Entity> root;

private:
	Weak<Entity> entitySelectedInHierarchy;
};
