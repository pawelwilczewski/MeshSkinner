#pragma once

#include "Application/Core.h"
#include "VertexArray.h"
#include "Core/Mesh/MeshComponent.h"
#include "Core/Mesh/StaticMeshComponent.h"
#include "Core/Mesh/SkeletalMeshComponent.h"
#include "Core/Entity/Transform.h"
#include "Core/Entity/Entity.h"
#include "Core/Camera/Camera.h"
#include "Core/Renderer/Buffer/StorageBuffer.h"

struct VertexInfo
{
	VertexInfo(GLuint transformIndex = -1, GLuint skeletonBonesIndex = -1, GLuint skeletonTransformsIndex = -1);

	GLuint transformIndex;
	GLuint skeletonBonesIndex;
	GLuint skeletonTransformsIndex;

	static const BufferLayout &layout;
};

using DrawCalls = std::map<Ref<Shader>, Ref<VertexArray<uint32_t>>>;

class Renderer
{
public:
	static void Init();

	static void Submit(Entity *entity);
	// TODO: add Remove which should be called upon destruction of MeshComponent owning entity - shouldn't just use weak_ptrs instead and clean the up accordingly?

	static void FrameBegin();
	static void FrameEnd();

private:
	static void SubmitMesh(Entity *entity, const MeshComponent *mesh, DrawCalls &drawCalls);
	static void SubmitMesh(Entity *entity, const Ref<StaticMeshComponent> &mesh);
	static void SubmitMesh(Entity *entity, const Ref<SkeletalMeshComponent> &mesh);

	static void Render(const DrawCalls::iterator &it);

public:
	static void UpdateMeshVertices(const MeshComponent *mesh);

	static std::vector<glm::vec4> GetFinalVertPosData(const MeshComponent *mesh);

public:
	inline static Camera *activeCamera = nullptr;
	inline static int selectedBone = 0; // TODO: make this available as static field in Hierarchy

private:
	inline static DrawCalls staticMeshDrawCalls;
	inline static DrawCalls skeletalMeshDrawCalls;

private:
	// key: entity already rendered, value: transform id to use for rendering
	inline static std::unordered_map<Entity *, const uint32_t> entities;

	// key: mesh already rendered, value: offset in vbo where the vertices start
	inline static std::unordered_map<const MeshComponent *, const uint32_t> meshes;

	inline static Unique<StorageBuffer<glm::mat4>> transforms;
	inline static Unique<StorageBuffer<BoneGPU>> bones;
	inline static Unique<StorageBuffer<glm::vec4>> finalPos; // todo: needs to be there per vao
};
