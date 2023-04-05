#pragma once

struct AnimationInfo;

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

	// SELECTION
public:
    Entity *GetSelectedEntity() const;
    SkeletalMeshComponent *GetSelectedSkeletalMesh() const;
    Bone *GetSelectedBone() const;
    uint32_t GetSelectedBoneIndex() const;

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
	void UpdateSelection(Entity *entity);
    void UpdateSelectedBone(uint32_t boneIndex);

	// ANIMATIONS
public:
	const std::vector<Animation> &GetAnimations() const;
	std::unordered_map<SkeletalMeshComponent *, AnimationInfo> &GetAnimationsRaw();
	void ImportAnimations(const std::string &path, SkeletalMeshComponent *mesh);

private:
	Entity *selectedEntity = nullptr;
	SkeletalMeshComponent *selectedSkeletalMesh = nullptr;
	Bone *selectedBone = nullptr;

	std::unordered_map<SkeletalMeshComponent *, AnimationInfo> animations;

	// SETTINGS
public:
	float tipBoneLength = 50.f;
	float boneRadius = 5.f;
};

struct AnimationInfo
{
	std::vector<Animation> animations;
	bool play = true;
	float playbackTime = 0.f;
	int animationIndex = 0;
};
