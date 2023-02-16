#pragma once

#include "Application/Core.h"

class MeshLibrary
{
public:
	static Ref<StaticMesh> GetCube();

public:
	static bool Get(const std::string &path, Ref<StaticMesh> &outMesh);
	static bool Get(const std::string &path, Ref<Skeleton> &outSkeleton, Ref<SkeletalMesh> &outMesh);

private:
	static std::unordered_map<std::string, Ref<StaticMesh>> staticMeshes;
	static std::unordered_map<std::string, std::pair<Ref<Skeleton>, Ref<SkeletalMesh>>> skeletalMeshes;
};
