#pragma once

class MeshLibrary
{
public:
	static Ref<StaticMeshComponent> GetCube();
	static Ref<StaticMeshComponent> GetBone(float length);

public:
	static Ref<StaticMeshComponent> Import(const std::string &path);
	static Ref<SkeletalMeshComponent> Import(const std::string &path, Scene *scene, Entity *entity);
	static bool Import(const std::string &path, std::vector<Animation> &outAnimations);

public:
	static void ExportUpdated(const std::string &source, const std::string &target, const SkeletalMeshComponent *inMesh);
};
