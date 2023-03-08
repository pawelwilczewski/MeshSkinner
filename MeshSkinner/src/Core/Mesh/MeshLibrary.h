#pragma once

#include "Application/Core.h"

class MeshLibrary
{
public:
	static Ref<StaticMesh> GetCube();
	static Ref<StaticMesh> GetBone(float length);

public:
	static bool Import(const std::string &path, Ref<StaticMesh> &outMesh);
	static bool Import(const std::string &path, Ref<SkeletalMesh> &outMesh, Ref<Bone> &outRoot);
	static bool Import(const std::string &path, std::vector<Animation> &outAnimations);

public:
	static void ExportUpdated(const std::string &source, const std::string &target, const Ref<SkeletalMesh> &inMesh);
};
