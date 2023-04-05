#pragma once

// singleton context class for ease of use
class Context
{
public:
    static Context &Get();

public:
    Context() = default;
    ~Context() = default;

public:
    Context(Context const &) = delete;
    void operator=(Context const &) = delete;

public:
    Entity *GetSelectedEntity() const;
    SkeletalMeshComponent *GetSelectedSkeletalMesh() const;
    Bone *GetSelectedBone() const;
    uint32_t GetSelectedBoneIndex() const;

	template <typename T>
	Ref<T> GetSelectedComponent()
	{
		if (!selectedEntity)
			return nullptr;

		auto &components = selectedEntity->GetComponents<T>();
		if (components.size() == 0)
			return nullptr;

		return *components.begin();
	}

public:
	void UpdateSelection(Entity *entity);
    void UpdateSelectedBone(uint32_t boneIndex);

private:
	Entity *selectedEntity = nullptr;
	SkeletalMeshComponent *selectedSkeletalMesh = nullptr;
	Bone *selectedBone = nullptr;
};
