#include "pch.h"
#include "Renderer.h"

VertexInfo::VertexInfo(uint32_t transformID, uint32_t materialID) : transformID(transformID), materialID(materialID)
{

}

DrawCallInfo::DrawCallInfo() :
	vao(MakeUnique<VertexArray<uint32_t>>()),
	transforms(MakeUnique<StorageBuffer<glm::mat4>>()),
	materials(MakeUnique<StorageBuffer<MaterialGPU>>()),
	vertexInfo(MakeUnique<StorageBuffer<VertexInfo>>()),
	entities(std::unordered_map<Ref<Entity>, const uint32_t>()),
	skeletons(std::unordered_map<Ref<Skeleton>, const uint32_t>())
{

}

Ref<Camera> Renderer::activeCamera;

DrawCalls Renderer::staticMeshDrawCallsStatic;
DrawCalls Renderer::skeletalMeshDrawCallsStatic;

DrawCalls Renderer::staticMeshDrawCallsDynamic;
DrawCalls Renderer::skeletalMeshDrawCallsDynamic;

void Renderer::Init()
{
	staticMeshDrawCallsStatic = DrawCalls();
	staticMeshDrawCallsStatic = DrawCalls();

	skeletalMeshDrawCallsDynamic = DrawCalls();
	skeletalMeshDrawCallsDynamic = DrawCalls();
}

void Renderer::SubmitMeshStatic(const Ref<Entity> &entity, const Mesh *mesh, DrawCalls &drawCalls, std::function<void(VertexArray<uint32_t> &)> vaoInitFunction, std::function<void(VertexArray<uint32_t> &)> fillVertexBufferFunction)
{
	// insert new shader if necessary
	if (drawCalls.find(mesh->material->shader) == drawCalls.end())
	{
		// new shader - create an appropriate vao
		auto drawCallInfo = MakeRef<DrawCallInfo>();

		vaoInitFunction(*drawCallInfo->vao.get());

		// calculate the insert index by comparing the depth value of each shader
		auto insertIndex = drawCalls.begin();
		for (const auto &drawCall : drawCalls)
		{
			if (drawCall.first->GetDepth() > mesh->material->shader->GetDepth())
				break;
			insertIndex++;
		}

		drawCalls.insert(insertIndex, {mesh->material->shader, drawCallInfo});
	}

	auto &vao = drawCalls[mesh->material->shader]->vao;
	auto &entities = drawCalls[mesh->material->shader]->entities;
	auto &vertexInfo = drawCalls[mesh->material->shader]->vertexInfo;
	auto &transforms = drawCalls[mesh->material->shader]->transforms;
	auto &materials = drawCalls[mesh->material->shader]->materials;

	// get the transform id
	uint32_t transformID;
	if (entities.find(entity) == entities.end())
	{
		// transform info update
		transforms->AppendData(&entity->GetWorldMatrix(), 1);
		transformID = transforms->GetLength() - 1;

		// new entity - insert for future use
		entities.insert({ entity, transformID });
	}
	else
	{
		transformID = entities[entity];
	}

	// fet the index offset, fill in the vbo
	uint32_t indexOffset = vao->GetVertexBuffer(0)->GetLength();
	fillVertexBufferFunction(*vao.get());

	// offset the indices appropriately
	std::vector<uint32_t> indicesOffset = mesh->indices;
	if (indexOffset > 0)
		for (auto &index : indicesOffset)
			index += indexOffset;

	// append the indices to the ibo
	auto &ibo = vao->GetIndexBuffer();
	ibo->AppendData(indicesOffset.data(), indicesOffset.size());

	// append the material data
	auto mat = MaterialGPU(*mesh->material.get());
	materials->AppendData(&mat, 1);

	// append the vertex info
	auto materialID = materials->GetLength() - 1;
	auto ids = std::vector<VertexInfo>(vao->GetVertexBuffer(0)->GetLength() - indexOffset, VertexInfo(transformID, materialID));
	vertexInfo->AppendData(ids.data(), ids.size());
}

void Renderer::SubmitMeshStatic(const Ref<Entity> &entity, const Ref<StaticMesh> &mesh)
{
	SubmitMeshStatic(entity, mesh.get(), staticMeshDrawCallsStatic,
		[&](VertexArray<uint32_t> &vao)
		{
			// initialize the ibo and vbo
			auto ibo = MakeRef<IndexBuffer<uint32_t>>();
			auto vbo = MakeRef<VertexBuffer<StaticVertex>>(StaticVertex::layout);
			vao.SetVertexBuffer(vbo, 0);
			vao.SetIndexBuffer(ibo);
		},
		[&](VertexArray<uint32_t> &vao)
		{
			// append the vertices to the vbo
			auto vbo = TypedVB<StaticVertex>(vao.GetVertexBuffer(0).get());
		vbo->SetData(mesh->vertices.data(), mesh->vertices.size(), vbo->GetLength());
		});
}

void Renderer::SubmitMeshStatic(const Ref<Entity> &entity, const Ref<SkeletalMesh> &mesh)
{
	SubmitMeshStatic(entity, mesh.get(), skeletalMeshDrawCallsStatic,
		[&](VertexArray<uint32_t> &vao)
		{
			// initialize the ibo and vbo
			auto ibo = MakeRef<IndexBuffer<uint32_t>>();
			auto vbo = MakeRef<VertexBuffer<SkeletalVertex>>(SkeletalVertex::layout);
			vao.SetVertexBuffer(vbo, 0);
			vao.SetIndexBuffer(ibo);
		},
		[&](VertexArray<uint32_t> &vao)
		{
			// append the vertices to the vbo
			auto vbo = TypedVB<SkeletalVertex>(vao.GetVertexBuffer(0).get());
			vbo->SetData(mesh->vertices.data(), mesh->vertices.size(), vbo->GetLength());
		});

	auto &skeletons = skeletalMeshDrawCallsStatic[mesh->material->shader]->skeletons;
	auto &transforms = skeletalMeshDrawCallsStatic[mesh->material->shader]->transforms;

	// submit all bones
	if (skeletons.find(mesh->skeleton) == skeletons.end())
	{
		uint32_t skeletonTransformOffset = transforms->GetLength();
		skeletons.insert({ mesh->skeleton, skeletonTransformOffset });

		for (auto &bone : mesh->skeleton->bones)
			Submit(bone);
	}
	else
	{
		Log::Error("Trying to render the same skeleton more than once!");
	}
}

void Renderer::Submit(const Ref<Entity> &entity)
{
	// submit all static meshes
	auto staticMeshes = entity->GetComponents<StaticMesh>();
	for (const auto &mesh : staticMeshes)
	{
		if (!mesh->isStatic)
			continue;

		SubmitMeshStatic(entity, mesh);
	}

	// submit all skeletal meshes
	auto skeletalMeshes = entity->GetComponents<SkeletalMesh>();
	for (auto &mesh : skeletalMeshes)
	{
		if (!mesh->isStatic) continue;

		SubmitMeshStatic(entity, mesh);
	}
}

void Renderer::FrameBegin()
{
	// TODO: submit all dynamic meshes
}

static void UpdateTransforms(const DrawCallInfo *info, std::unordered_set<const Entity *> &entitiesToUpdate)
{
	// update the transforms if necessary
	for (auto &[entity, transformID] : info->entities)
	{
		if (!entity->GetIsWorldMatrixUpdated() || entitiesToUpdate.find(entity.get()) != entitiesToUpdate.end())
		{
			info->transforms->SetData(&entity->GetWorldMatrix(), 1, transformID);
			entitiesToUpdate.insert(entity.get());
		}
	}
}

void Renderer::Render(const DrawCalls::iterator &it)
{
	const auto &shader = it->first;
	const auto &info = it->second;

	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", activeCamera->GetViewProjectionMatrix());
	// TODO: probably id shouldn't be necessary (similar to uploading uniforms)
	shader->SetupStorageBuffer("ss_VertexInfo", info->vertexInfo->GetID());
	shader->SetupStorageBuffer("ss_Transforms", info->transforms->GetID());
	//shader->SetupStorageBuffer("ss_Materials", info->materials->GetID());

	info->vao->Bind();
	glDrawElements(GL_TRIANGLES, info->vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::FrameEnd()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::unordered_set<const Entity *> entitiesToUpdate;

	for (const auto &[shader, info] : staticMeshDrawCallsStatic)
		UpdateTransforms(info.get(), entitiesToUpdate);

	for (const auto &[shader, info] : skeletalMeshDrawCallsStatic)
		UpdateTransforms(info.get(), entitiesToUpdate);

	// render all draw calls respecting the depth for each shader (drawCalls are already sorted)

	std::vector<std::pair<DrawCalls::iterator, DrawCalls::iterator>> drawCallIterators = {
		{ staticMeshDrawCallsStatic.begin(), staticMeshDrawCallsStatic.end() },
		{ skeletalMeshDrawCallsStatic.begin(), skeletalMeshDrawCallsStatic.end() }
	};

	uint16_t depth = 0;
	while (true)
	{
		// render each draw calls element
		bool finished = true;
		for (auto &iterator : drawCallIterators)
		{
			while (iterator.first != iterator.second && iterator.first->first->GetDepth() <= depth)
				Render(iterator.first++);

			if (iterator.first != iterator.second)
				finished = false;
		}

		// break if all finished
		if (finished)
			break;

		// update the depth
		uint16_t minDepth = -1;
		for (const auto &iterator : drawCallIterators)
		{
			if (iterator.first != iterator.second && iterator.first->first->GetDepth() < minDepth)
				minDepth = iterator.first->first->GetDepth();
		}

		depth = minDepth;

		// clear the depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}
