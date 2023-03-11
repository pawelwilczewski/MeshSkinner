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

	template <typename T>
	Ref<T> GetSelectedComponent() const
	{
		if (!selectedEntity)
			return nullptr;

		auto &components = selectedEntity->GetComponents<T>();
		if (components.size() == 0)
			return nullptr;

		return *components.begin();
	}

public:
	Entity *root;

private:
	Entity *selectedEntity = nullptr;
};
