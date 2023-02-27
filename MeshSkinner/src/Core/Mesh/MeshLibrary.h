#pragma once

#include "Application/Core.h"

class MeshLibrary
{
public:
	static Ref<StaticMesh> GetCube();
	static Ref<StaticMesh> GetBone(float length);

public:
	static bool Get(const std::string &path, Ref<StaticMesh> &outMesh);
	static bool Get(const std::string &path, Ref<SkeletalMesh> &outMesh, Ref<Bone> &outRoot);
};
