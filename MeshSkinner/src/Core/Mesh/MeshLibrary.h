#pragma once

#include "Application/Core.h"

class MeshLibrary
{
public:
	static Ref<StaticMesh> GetCube();

private:
	static std::unordered_map<std::string, Ref<StaticMesh>> staticMeshes;
	static std::unordered_map<std::string, Ref<SkeletalMesh>> skeletalMeshes;
};
