#pragma once

#include "Application/Core.h"

class MeshLibrary
{
public:
	static Ref<StaticMeshComponent> GetCube();
	static Ref<StaticMeshComponent> GetBone(float length);

public:
	static bool Import(const std::string &path, Ref<StaticMeshComponent> &outMesh);
	static bool Import(const std::string &path, Ref<SkeletalMeshComponent> &outMesh, Ref<Bone> &outRoot);
	static bool Import(const std::string &path, std::vector<Animation> &outAnimations);

public:
	static void ExportUpdated(const std::string &source, const std::string &target, const Ref<SkeletalMeshComponent> &inMesh);
};
