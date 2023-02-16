#pragma once

#include "Application/Core.h"

class MeshLibrary
{
public:
	static Ref<StaticMesh> GetCube();

public:
	static bool Get(const std::string &path, Ref<StaticMesh> &mesh);
	static bool Get(const std::string &path, Ref<Skeleton> &skeleton, Ref<SkeletalMesh> &mesh);

private:
	static std::unordered_map<std::string, Ref<StaticMesh>> staticMeshes;
	static std::unordered_map<std::string, std::pair<Ref<Skeleton>, Ref<SkeletalMesh>>> skeletalMeshes;
};
