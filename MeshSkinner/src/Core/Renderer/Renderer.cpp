#include "pch.h"
#include "Renderer.h"

VertexInfo::VertexInfo(uint32_t transformID, uint32_t materialID, uint32_t skeletonID) : transformID(transformID), materialID(materialID), skeletonID(skeletonID)
{

}

DrawCallInfo::DrawCallInfo() :
	vao(MakeUnique<VertexArray<uint32_t>>()),
	transforms(MakeUnique<StorageBuffer<glm::mat4>>()),
	materials(MakeUnique<StorageBuffer<MaterialGPU>>()),
	vertexInfo(MakeUnique<StorageBuffer<VertexInfo>>()),
	bones(MakeUnique<StorageBuffer<BoneGPU>>()),
	finalPos(MakeUnique<StorageBuffer<glm::vec4>>())
{

}

Camera *Renderer::activeCamera;
int Renderer::activeBone = 0;

DrawCalls Renderer::staticMeshDrawCalls;
DrawCalls Renderer::skeletalMeshDrawCalls;

void Renderer::Init()
{

}

void Renderer::SubmitMesh(Entity *entity, const MeshComponent *mesh, DrawCalls &drawCalls, bool skeletal)
{
	// insert new shader if necessary
	if (drawCalls.find(mesh->material->shader) == drawCalls.end())
	{
		// new shader - create an appropriate vao
		auto drawCallInfo = MakeRef<DrawCallInfo>();

		// initialize the ibo
		auto ibo = MakeRef<IndexBuffer<uint32_t>>();
		drawCallInfo->vao->SetIndexBuffer(ibo);

		// initialize the vbo
		// TODO: probably no need to switch here, instead, i.e. adjust the code and instantiate GenericVertexBuffer
		Ref<GenericVertexBuffer> vbo;
		switch (mesh->GetVertexType())
		{
		case MeshComponent::VertexType::Static:
			vbo = MakeRef<VertexBuffer<StaticVertex>>(mesh->GetVertexBufferLayout());
			break;
		case MeshComponent::VertexType::Skeletal:
			vbo = MakeRef<VertexBuffer<SkeletalVertex>>(mesh->GetVertexBufferLayout());
			break;
		default:
			assert(false);
		}
		drawCallInfo->vao->SetVertexBuffer(vbo, 0);

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
	auto &meshes = drawCalls[mesh->material->shader]->meshes;
	auto &bones = drawCalls[mesh->material->shader]->bones;
	auto &finalPos = drawCalls[mesh->material->shader]->finalPos;

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

	// append the vertices to the vbo
	auto vbo = dynamic_cast<GenericBuffer *>(vao->GetVertexBuffer(0).get());
	auto vboOffsetLength = vbo->GetSizeBytes() / mesh->GetVertexBufferLayout().GetStride();
	vbo->SetData(mesh->GetVertices(), (GLuint)(mesh->GetVerticesLength() * mesh->GetVertexBufferLayout().GetStride()), vbo->GetSizeBytes());

	// init final pos
	std::vector<glm::vec4> finalPosInit(vao->GetVertexBuffer(0)->GetLength(), glm::vec4(0.f));
	finalPos->AppendData(finalPosInit.data(), finalPosInit.size());

	// update meshes offset info
	if (meshes.find(mesh) == meshes.end())
	//	assert(false); // duplicate mesh added
	//else
		meshes.insert({ mesh, vboOffsetLength });

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
	auto ids = std::vector<VertexInfo>(vao->GetVertexBuffer(0)->GetLength() - indexOffset, VertexInfo(transformID, materialID, skeletal ? bones->GetLength() : -1));
	vertexInfo->AppendData(ids.data(), ids.size());
}

void Renderer::SubmitMesh(Entity *entity, const Ref<StaticMeshComponent> &mesh)
{
	SubmitMesh(entity, mesh.get(), staticMeshDrawCalls);
}

void Renderer::SubmitMesh(Entity *entity, const Ref<SkeletalMeshComponent> &mesh)
{
	// submit the mesh
	SubmitMesh(entity, mesh.get(), skeletalMeshDrawCalls, true);

	// submit the skeleton (all bones)
	auto &skeletons = skeletalMeshDrawCalls[mesh->material->shader]->skeletons;
	auto &bones = skeletalMeshDrawCalls[mesh->material->shader]->bones;
	if (skeletons.find(mesh->skeleton.get()) == skeletons.end())
	{
		skeletons.insert({ mesh->skeleton.get(), bones->GetLength()});

		auto inverseRoot = glm::inverse(mesh->skeleton->GetRootBone()->GetWorldMatrix());
		for (auto &bone : mesh->skeleton->GetBones())
		{
			auto bonegpu = BoneGPU(*bone, inverseRoot);
			bones->AppendData(&bonegpu, 1);
			Submit(bone);
		}
	}
	else
	{
		Log::Info("Trying to render the same skeleton more than once! Skipping this mesh (perhaps mesh attached to the root bone).");

		return;
	}
}

void Renderer::Submit(Entity *entity)
{
	// submit all static meshes
	auto staticMeshes = entity->GetComponents<StaticMeshComponent>();
	for (const auto &mesh : staticMeshes)
		SubmitMesh(entity, mesh);

	// submit all skeletal meshes
	auto skeletalMeshes = entity->GetComponents<SkeletalMeshComponent>();
	for (auto &mesh : skeletalMeshes)
		SubmitMesh(entity, mesh);
}

void Renderer::FrameBegin()
{
}

static void UpdateTransforms(const Ref<DrawCallInfo> &info, std::unordered_set<const Entity *> &entitiesToUpdate)
{
	// update the transforms if necessary
	for (auto &[entity, transformID] : info->entities)
	{
		if (!entity->GetIsWorldMatrixUpdated() || entitiesToUpdate.find(entity) != entitiesToUpdate.end())
		{
			info->transforms->SetData(&entity->GetWorldMatrix(), 1, transformID);
			entitiesToUpdate.insert(entity);
		}
	}

	// update all bones
	for (auto &[skeleton, skeletonID] : info->skeletons)
	{
		auto inverseRoot = glm::inverse(skeleton->GetRootBone()->GetWorldMatrix());
		for (int i = 0; i < skeleton->GetBones().size(); i++)
		{
			if (!skeleton->GetBones()[i]->GetIsWorldMatrixUpdated())
			{
				auto bonegpu = BoneGPU(*skeleton->GetBones()[i], inverseRoot);
				info->bones->SetData(&bonegpu, 1, skeletonID + i);
				entitiesToUpdate.insert(skeleton->GetBones()[i]);
			}
		}
	}
}

void Renderer::Render(const DrawCalls::iterator &it)
{
	const auto &shader = it->first;
	const auto &info = it->second;

	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", activeCamera->GetViewProjectionMatrix());
	shader->UploadUniformInt("u_ActiveBone", activeBone);
	// TODO: probably id shouldn't be necessary (similar to uploading uniforms)
	shader->SetupStorageBuffer("ss_VertexInfo", info->vertexInfo->GetID());
	shader->SetupStorageBuffer("ss_Transforms", info->transforms->GetID());
	shader->SetupStorageBuffer("ss_Bones", info->bones->GetID());
	shader->SetupStorageBuffer("ss_FinalPos", info->finalPos->GetID());
	//shader->SetupStorageBuffer("ss_Materials", info->materials->GetID());

	info->vao->Bind();
	glDrawElements(GL_TRIANGLES, info->vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::UpdateMeshVertices(const MeshComponent *mesh)
{
	DrawCalls drawCalls;
	switch (mesh->GetVertexType())
	{
	case MeshComponent::VertexType::Static:
		drawCalls = staticMeshDrawCalls;
		break;
	case MeshComponent::VertexType::Skeletal:
		drawCalls = skeletalMeshDrawCalls;
		break;
	}

	auto &meshes = drawCalls[mesh->material->shader]->meshes;
	auto &vao = drawCalls[mesh->material->shader]->vao;
	
	// append the vertices to the vbo
	auto vbo = dynamic_cast<GenericBuffer *>(vao->GetVertexBuffer(0).get());
	auto stride = mesh->GetVertexBufferLayout().GetStride();
	vbo->SetData(mesh->GetVertices(), (GLuint)(mesh->GetVerticesLength() * stride), meshes[mesh] * stride);
}

void Renderer::FrameEnd()
{
	// clear the screen before rendering anything
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// helper iterators used for rendering with correct depth - insert other DrawCalls here to effectively dispatch them
	std::vector<std::pair<DrawCalls::iterator, DrawCalls::iterator>> drawCallIterators = {
		{ staticMeshDrawCalls.begin(), staticMeshDrawCalls.end() },
		{ skeletalMeshDrawCalls.begin(), skeletalMeshDrawCalls.end() }
	};

	// helper array to ensure all transforms are updated (some entities have static and skeletal meshes
	//  - transform needs to be updated for entities in all draw calls)
	std::unordered_set<const Entity *> entitiesToUpdate;

	// render all draw calls respecting the depth for each shader (drawCalls are already sorted)
	uint16_t depth = 0;
	while (true)
	{
		// dispatch all draw calls
		bool finished = true;
		for (auto &iterator : drawCallIterators)
		{
			while (iterator.first != iterator.second && iterator.first->first->GetDepth() <= depth)
			{
				// update all transforms before rendering
				UpdateTransforms(iterator.first->second, entitiesToUpdate);

				// render
				Render(iterator.first++);
			}

			// not finished if anything left
			if (iterator.first != iterator.second)
				finished = false;
		}

		// break if all finished
		if (finished)
			break;

		// update the depth
		uint16_t minDepth = -1;
		for (const auto &iterator : drawCallIterators)
			if (iterator.first != iterator.second && iterator.first->first->GetDepth() < minDepth)
				minDepth = iterator.first->first->GetDepth();

		depth = minDepth;

		// clear the depth buffer
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}
