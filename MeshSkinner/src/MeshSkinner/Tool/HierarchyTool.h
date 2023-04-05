#pragma once

#include "Tool.h"

class HierarchyTool : public Tool
{
public:
	HierarchyTool(const std::string &toolWindowName, Entity *root);
	virtual ~HierarchyTool() = default;

public:
	virtual void OnUpdateUI() override;

private:
	void DrawTree(Entity *entity);

public:
	Entity *root;
};
