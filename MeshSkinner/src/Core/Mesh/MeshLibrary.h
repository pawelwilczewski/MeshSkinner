#pragma once

class MeshLibrary
{
public:
	static Ref<StaticMeshComponent> GetCube();
	static Ref<StaticMeshComponent> GetBone(float length);

public:
	static Ref<StaticMeshComponent> Import(const std::string &path, bool triggerEvent = true);
	static Ref<SkeletalMeshComponent> Import(const std::string &path, Scene *scene, Entity *entity);
	static bool Import(const std::string &path, std::vector<Animation> &outAnimations);

public:
	static void ExportUpdated(const std::string &source, const std::string &target, const SkeletalMeshComponent *inMesh);

public:
	static void OnStaticMeshImportedSubscribe(const CallbackRef<StaticMeshComponent *> &callback);
	static void OnStaticMeshImportedUnsubscribe(const CallbackRef<StaticMeshComponent *> &callback);
	static void OnSkeletalMeshImportedSubscribe(const CallbackRef<SkeletalMeshComponent *> &callback);
	static void OnSkeletalMeshImportedUnsubscribe(const CallbackRef<SkeletalMeshComponent *> &callback);

private:
	inline static Event<StaticMeshComponent *> onStaticMeshImported;
	inline static Event<SkeletalMeshComponent *> onSkeletalMeshImported;
};
