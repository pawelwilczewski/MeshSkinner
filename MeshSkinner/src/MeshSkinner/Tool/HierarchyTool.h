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
	static Entity *GetSelectedEntity();

	template <typename T>
	static Ref<T> GetSelectedComponent()
	{
		if (!selectedEntity)
			return nullptr;

		auto &components = selectedEntity->GetComponents<T>();
		if (components.size() == 0)
			return nullptr;

		return *components.begin();
	}

	static Bone *GetSelectedBone();
	static SkeletalMeshComponent *GetSelectedSkeletalMesh();
	static uint32_t GetSelectedBoneIndex();

	static void UpdateSelectedBone(uint32_t boneIndex);
	static void UpdateSelectedEntity(Entity *entity);

public:
	Entity *root;

private:
	inline static Entity *selectedEntity = nullptr;
	inline static SkeletalMeshComponent *selectedSkeletalMesh = nullptr;
	inline static Bone *selectedBone = nullptr;
};
