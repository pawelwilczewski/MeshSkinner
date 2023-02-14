#include "pch.h"
#include "Renderer.h"

DrawCallInfo::DrawCallInfo() :
	vao(MakeUnique<VertexArray<uint32_t>>()),
	transforms(MakeUnique<StorageBuffer<glm::mat4>>()),
	materials(MakeUnique<StorageBuffer<MaterialGPU>>()),
	vertexInfo(MakeUnique<StorageBuffer<VertexInfo>>()),
	meshes(std::unordered_map<const Mesh &, const uint32_t>())
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

void Renderer::SubmitMeshStatic(Entity &entity, const Mesh &mesh, DrawCalls &drawCalls, std::function<void(VertexArray<uint32_t> &)> vaoInitFunction, std::function<uint32_t(VertexArray<uint32_t> &)> fillVertexBufferFunction)
{
	auto &vao = drawCalls[mesh.material->shader]->vao;
	auto &entities = drawCalls[mesh.material->shader]->entities;
	auto &meshes = drawCalls[mesh.material->shader]->meshes;
	auto &vertexInfo = drawCalls[mesh.material->shader]->vertexInfo;
	auto &transforms = drawCalls[mesh.material->shader]->transforms;
	auto &materials = drawCalls[mesh.material->shader]->materials;
	// TODO: what if that drawCalls element does not exist yet?

	// ensure no duplicates
	if (std::find(entities.begin(), entities.end(), entity) != entities.end())
		return;

	// transform info update
	entities.push_back(entity);
	transforms->AppendData(&entity.transform.GetMatrix(), 1);
	auto transformID = transforms->GetLength() - 1;

	uint32_t indexOffset;
	// the mesh is already rendered - reuse its data
	if (meshes.find(mesh) != meshes.end())
	{
		// reuse the cached index offset
		indexOffset = meshes[mesh];
	}
	else // new mesh
	{
		// new shader - create an appropriate vao
		if (drawCalls.find(mesh.material->shader) == drawCalls.end())
		{
			auto drawCallInfo = MakeRef<DrawCallInfo>();

			vaoInitFunction(*drawCallInfo->vao.get());

			drawCalls.insert({ mesh.material->shader, drawCallInfo });
		}

		indexOffset = fillVertexBufferFunction(*vao.get());

		// add the meshes to the map for reuse later
		meshes.insert({ mesh, indexOffset });
	}

	// add indices (appropriately offset)
	std::vector<uint32_t> indicesOffset = mesh.indices;
	if (indexOffset > 0)
		for (auto &index : indicesOffset)
			index += indexOffset;

	auto &ibo = vao->GetIndexBuffer();
	ibo->AppendData(indicesOffset.data(), indicesOffset.size());

	// add the material
	materials->AppendData(&MaterialGPU(*mesh.material.get()), 1);

	// add the vertex info
	auto materialID = materials->GetLength() - 1;
	auto ids = std::vector<VertexInfo>(indicesOffset.size(), { transformID, materialID });
	vertexInfo->AppendData(ids.data(), ids.size());
}

void Renderer::Submit(Ref<Entity> entity)
{
	// submit all static meshes
	auto staticMeshes = entity->GetComponents<StaticMesh>();
	for (auto &mesh : staticMeshes)
	{
		if (!mesh.isStatic) continue;

		SubmitMeshStatic(
			*entity.get(),
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
			[&](VertexArray<uint32_t> &vao) -> uint32_t
			{
				// append the vertices to the vbo
				auto vbo = TypedVB<StaticVertex>(vao.GetVertexBuffer(0).get());
				auto indexOffset = vbo->GetLength();
				vbo->SetData(mesh.vertices.data(), mesh.vertices.size(), vbo->GetLength());
				return indexOffset;
			});
	}
	
	auto skeletalMeshes = entity->GetComponents<SkeletalMesh>();
	for (auto &mesh : skeletalMeshes)
	{
		if (!mesh.isStatic) continue;

		SubmitMeshStatic(
			*entity.get(),
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
			[&](VertexArray<uint32_t> &vao) -> uint32_t
			{
				// append the vertices to the vbo
				auto vbo = TypedVB<SkeletalVertex>(vao.GetVertexBuffer(0).get());
				auto indexOffset = vbo->GetLength();
				vbo->SetData(mesh.vertices.data(), mesh.vertices.size(), vbo->GetLength());
				return indexOffset;
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
		// update the transforms if necessary (TODO: probably batch this operation to decrease gpu calls?)
		for (int i = 0; i < info->entities.size(); ++i)
			if (!info->entities[i].transform.IsMatrixUpdated())
				info->transforms->SetData(&info->entities[i].transform.GetMatrix(), 1, i);

		// TODO: same for materials

		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", camera->GetViewProjectionMatrix());
		// TODO: probably id shouldn't be necessary (similar to uploading uniforms)
		shader->SetupStorageBuffer("ss_VertexInfo", 0, info->vertexInfo->GetID());
		shader->SetupStorageBuffer("ss_Transforms", 1, info->transforms->GetID());
		//shader->SetupStorageBuffer("ss_Materials", 2, materialsStatic->GetID());

		info->vao->Bind();
		glDrawElements(GL_TRIANGLES, info->vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
	}
}

void Renderer::FrameEnd()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderDrawCalls(activeCamera, staticMeshDrawCallsStatic);
	RenderDrawCalls(activeCamera, skeletalMeshDrawCallsStatic);
}
