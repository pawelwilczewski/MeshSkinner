#pragma once

#include "Application/Core.h"
#include "Tool.h"

struct AnimationInfo
{
	std::vector<Animation> animations;
	bool play = true;
	float playbackTime = 0.f;
	int animationIndex = 0;
};

class AnimationControlsTool : public Tool
{
public:
	AnimationControlsTool(const std::string &toolWindowName = "Animation Controls");
	virtual ~AnimationControlsTool();

public:
	const std::vector<Animation> &GetAnimations() const;

protected:
	virtual void OnUpdateUI() override;
	void OnUpdate();

public:
	static void ImportAnimations(const std::string &path, SkeletalMeshComponent *mesh);

private:
	std::string sourceFile;

	static inline std::unordered_map<SkeletalMeshComponent *, AnimationInfo> animations;
	CallbackNoArgRef onUpdateCallback;
};
