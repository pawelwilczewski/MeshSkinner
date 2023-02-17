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
	meshes(std::unordered_set<const Mesh *>())
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

		drawCalls.insert({ mesh->material->shader, drawCallInfo });
	}

	auto &vao = drawCalls[mesh->material->shader]->vao;
	auto &entities = drawCalls[mesh->material->shader]->entities;
	auto &meshes = drawCalls[mesh->material->shader]->meshes;
	auto &vertexInfo = drawCalls[mesh->material->shader]->vertexInfo;
	auto &transforms = drawCalls[mesh->material->shader]->transforms;
	auto &materials = drawCalls[mesh->material->shader]->materials;

	// get the transform id
	uint32_t transformID;
	if (entities.find(entity) == entities.end())
	{
		// transform info update
		transforms->AppendData(&entity->transform.GetMatrix(), 1);
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

void Renderer::Submit(const Ref<Entity> &entity)
{
	// submit all static meshes
	auto staticMeshes = entity->GetComponents<StaticMesh>();
	for (const auto &mesh : staticMeshes)
	{
		if (!mesh->isStatic)
			continue;

		SubmitMeshStatic(
			entity,
			mesh,
			staticMeshDrawCallsStatic,
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
			}
			);
	}

	auto skeletalMeshes = entity->GetComponents<SkeletalMesh>();
	for (auto &mesh : skeletalMeshes)
	{
		if (!mesh->isStatic) continue;

		SubmitMeshStatic(
			entity,
			mesh,
			skeletalMeshDrawCallsStatic,
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
			}
		);
	}
}

void Renderer::FrameBegin()
{
	// TODO: submit all dynamic meshes
}

void Renderer::RenderDrawCalls(const Ref<Camera> &camera, const DrawCalls &drawCalls)
{
	for (const auto &[shader, info] : drawCalls)
	{
		// TODO: same for materials

		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		// TODO: probably id shouldn't be necessary (similar to uploading uniforms)
		shader->SetupStorageBuffer("ss_VertexInfo", info->vertexInfo->GetID());
		shader->SetupStorageBuffer("ss_Transforms", info->transforms->GetID());
		//shader->SetupStorageBuffer("ss_Materials", info->materials->GetID());

		info->vao->Bind();
		glDrawElements(GL_TRIANGLES, info->vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
	}
}

void UpdateTransforms(const DrawCallInfo *info, std::unordered_set<const Entity *> &entitiesToUpdate)
{
	// update the transforms if necessary
	for (auto &[entity, transformID] : info->entities)
	{
		if (!entity->transform.IsMatrixUpdated() || entitiesToUpdate.find(entity.get()) != entitiesToUpdate.end())
		{
			info->transforms->SetData(&entity->transform.GetMatrix(), 1, transformID);
			entitiesToUpdate.insert(entity.get());
		}
	}
}

void Renderer::FrameEnd()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::unordered_set<const Entity *> entitiesToUpdate;

	for (const auto &[shader, info] : staticMeshDrawCallsStatic)
		UpdateTransforms(info.get(), entitiesToUpdate);

	for (const auto &[shader, info] : skeletalMeshDrawCallsStatic)
		UpdateTransforms(info.get(), entitiesToUpdate);

	RenderDrawCalls(activeCamera, staticMeshDrawCallsStatic);
	RenderDrawCalls(activeCamera, skeletalMeshDrawCallsStatic);
}
