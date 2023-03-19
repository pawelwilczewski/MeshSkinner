#include "pch.h"
#include "Renderer.h"

#include "MeshSkinner/Tool/Hierarchy.h"

const BufferLayout &VertexInfo::layout = BufferLayout({
	{ "transformIndex", ShaderDataType::UnsignedInt },
	{ "skeletonBonesIndex", ShaderDataType::UnsignedInt },
	{ "skeletonTransformsIndex", ShaderDataType::UnsignedInt }
	});

VertexInfo::VertexInfo(GLuint transformIndex, GLuint skeletonBonesIndex, GLuint skeletonTransformsIndex) : transformIndex(transformIndex), skeletonBonesIndex(skeletonBonesIndex), skeletonTransformsIndex(skeletonTransformsIndex)
{

}

void Renderer::Init()
{
	transforms = MakeUnique<StorageBuffer<glm::mat4>>();
	bones = MakeUnique<StorageBuffer<BoneGPU>>();
	finalPos = MakeUnique<StorageBuffer<glm::vec4>>();
}

void Renderer::SubmitMesh(Entity *entity, const MeshComponent *mesh, DrawCalls &drawCalls)
{
	// insert new shader if necessary
	if (drawCalls.find(mesh->material->shader) == drawCalls.end())
	{
		// new shader - create an appropriate vao
		auto vao = MakeRef<VertexArray<uint32_t>>();

		// initialize the ibo
		auto ibo = MakeRef<IndexBuffer<uint32_t>>();
		vao->SetIndexBuffer(ibo);

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
		vao->SetVertexBuffer(vbo, 0);

		// add vertex info vbo
		vao->SetVertexBuffer(MakeRef<VertexBuffer<VertexInfo>>(VertexInfo::layout), 1);

		// calculate the insert index by comparing the depth value of each shader
		auto insertIndex = drawCalls.begin();
		for (const auto &drawCall : drawCalls)
		{
			if (drawCall.first->GetDepth() > mesh->material->shader->GetDepth())
				break;
			insertIndex++;
		}

		drawCalls.insert(insertIndex, { mesh->material->shader, vao });
	}

	auto &vao = drawCalls[mesh->material->shader];

	// get the transform index
	GLuint transformIndex;
	if (entities.find(entity) == entities.end())
	{
		// transform info update
		transformIndex = transforms->GetLength();
		transforms->AppendData(&entity->GetWorldMatrix(), 1);

		// new entity - insert for future use
		entities.insert({ entity, transformIndex });
	}
	else
	{
		transformIndex = entities[entity];
	}

	// set the index offset, fill in the vbo
	uint32_t indexOffset = vao->GetVertexBuffer(0)->GetLength();

	// append the vertices to the vbo
	auto vbo = dynamic_cast<GenericBuffer *>(vao->GetVertexBuffer(0).get());
	auto vboOffsetLength = vbo->GetSizeBytes() / mesh->GetVertexBufferLayout().GetStride();
	vbo->SetData(mesh->GetVertices(), (GLuint)(mesh->GetVerticesLength() * mesh->GetVertexBufferLayout().GetStride()), vbo->GetSizeBytes());

	// append the vertex info
	auto vboInfo = dynamic_cast<GenericBuffer *>(vao->GetVertexBuffer(1).get());
	auto infoData = std::vector<VertexInfo>(vao->GetVertexBuffer(0)->GetLength() - indexOffset, VertexInfo(transformIndex, bones->GetLength(), transforms->GetLength()));
	vboInfo->SetData(infoData.data(), (GLuint)(mesh->GetVerticesLength() * VertexInfo::layout.GetStride()), vboInfo->GetSizeBytes());

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
}

void Renderer::SubmitMesh(Entity *entity, const Ref<StaticMeshComponent> &mesh)
{
	SubmitMesh(entity, mesh.get(), staticMeshDrawCalls);
}

void Renderer::SubmitMesh(Entity *entity, const Ref<SkeletalMeshComponent> &mesh)
{
	// submit the mesh
	SubmitMesh(entity, mesh.get(), skeletalMeshDrawCalls);

	// submit the skeleton (all bones)
	for (auto &bone : mesh->skeleton->GetBones())
	{
		auto bonegpu = BoneGPU(*bone); // TODO: can be achieved using a single append call
		bones->AppendData(&bonegpu, 1);
		Submit(bone);
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

void Renderer::Render(const DrawCalls::iterator &it)
{
	const auto &shader = it->first;
	const auto &vao = it->second;

	//if (shader->GetDisplayedName() == "Bone")
	//{
	//	auto l = vao->GetVertexBuffer(0)->GetLength();
	//	std::vector<VertexInfo> testData(l, VertexInfo());
	//	vertexInfo->CopyData(0, l, testData.data());
	//	Log::Info("Vertex info for shader {}", shader->GetDisplayedName());
	//	for (size_t i = 0; i < l; i++)
	//		Log::Info("{}: t = {}, sb = {}, st = {}", i, testData[i].transformIndex, testData[i].skeletonBonesIndex, testData[i].skeletonTransformsIndex);
	//	Log::Info("------------------------------");
	//}

	shader->Bind();
	shader->UploadUniformMat4("u_ViewProjection", activeCamera->GetViewProjectionMatrix());
	GLint selected = -1;
	if (Hierarchy::GetSelectedEntity())
		selected = entities[Hierarchy::GetSelectedEntity()];
	shader->UploadUniformInt("u_SelectedEntity", selected);
	shader->UploadUniformInt("u_SelectedBone", selectedBone);
	shader->SetupStorageBuffer("ss_Transforms", transforms->GetID());
	shader->SetupStorageBuffer("ss_Bones", bones->GetID());
	shader->SetupStorageBuffer("ss_FinalPos", finalPos->GetID());

	vao->Bind();
	glDrawElements(GL_TRIANGLES, vao->GetIndexBuffer()->GetLength(), GL_UNSIGNED_INT, nullptr);
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

	auto &vao = drawCalls[mesh->material->shader];

	if (!vao)
	{
		Log::Warn("Mesh not rendered yet but attempting to update vertices!");
		return;
	}

	// update the vertices
	auto vbo = dynamic_cast<GenericBuffer *>(vao->GetVertexBuffer(0).get());
	auto stride = mesh->GetVertexBufferLayout().GetStride();
	vbo->SetData(mesh->GetVertices(), (GLuint)(mesh->GetVerticesLength() * stride), meshes[mesh] * stride);
}

void Renderer::FrameEnd()
{
	// clear the screen before rendering anything
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update the transforms
	for (auto &[mesh, _] : meshes)
	{
		auto entity = mesh->GetEntity();

		if (!entity->GetIsWorldMatrixUpdated())
			transforms->SetData(&entity->GetWorldMatrix(), 1, entities[entity]);
	}

	// helper iterators used for rendering with correct depth - insert other DrawCalls here to effectively dispatch them
	std::vector<std::pair<DrawCalls::iterator, DrawCalls::iterator>> drawCallIterators = {
		{ staticMeshDrawCalls.begin(), staticMeshDrawCalls.end() },
		{ skeletalMeshDrawCalls.begin(), skeletalMeshDrawCalls.end() }
	};

	// render all draw calls respecting the depth for each shader (drawCalls are already sorted)
	uint16_t depth = 0;
	while (true)
	{
		// dispatch all draw calls
		bool finished = true;
		for (auto &iterator : drawCallIterators)
		{
			// render
			while (iterator.first != iterator.second && iterator.first->first->GetDepth() <= depth)
				Render(iterator.first++);

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

std::vector<glm::vec4> Renderer::GetFinalVertPosData(const MeshComponent *mesh)
{
	std::vector<glm::vec4> result;
	result.resize(mesh->GetVerticesLength());
	finalPos->CopyData(meshes[mesh], mesh->GetVerticesLength(), result.data());

	return result;
}
