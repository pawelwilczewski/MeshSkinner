#include "pch.h"
#include "MeshLibrary.h"

#include "tiny_gltf.h"

std::unordered_map<std::string, Ref<StaticMesh>> MeshLibrary::staticMeshes;
std::unordered_map<std::string, std::pair<Ref<Skeleton>, Ref<SkeletalMesh>>> MeshLibrary::skeletalMeshes;

Ref<StaticMesh> MeshLibrary::GetCube()
{
	if (staticMeshes.find("Cube") == staticMeshes.end())
	{
		std::vector<StaticVertex> vertices;
		vertices.push_back(StaticVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.f)));
		vertices.push_back(StaticVertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.f)));
		vertices.push_back(StaticVertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.f)));
		vertices.push_back(StaticVertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f), glm::vec4(0.f), glm::vec2(0.f), glm::vec3(1.f)));

		std::vector<uint32_t> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		staticMeshes.insert({ "Cube", MakeRef<StaticMesh>(vertices, indices, MaterialLibrary::GetDefault(), true) });
	}

	return staticMeshes["Cube"];
}

static tinygltf::Model model;
static tinygltf::TinyGLTF loader;
static std::string err;
static std::string warn;

static bool LoadGLTF(const std::string &path)
{
	bool success = false;
	auto extension = std::filesystem::path(path).extension();

	if (extension == ".gltf")		success = loader.LoadASCIIFromFile(&model, &err, &warn, path);
	else if (extension == ".glb")	success = loader.LoadBinaryFromFile(&model, &err, &warn, path);

	if (!warn.empty())	Log::Error("GLTF Import: Warning: {}", path);
	if (!err.empty())	Log::Error("GLTF Import: Error: {}", path);
	if (!success)		Log::Error("GLTF Import: Failed to parse from file: {}", path);

	return success;
}

static bool LoadFromFile(const std::string &path)
{
	auto extension = std::filesystem::path(path).extension();

	if (extension == ".gltf" || extension == ".glb")
	{
		if (!LoadGLTF(path))
			return false;
	}
	else
	{
		Log::Error("Unsupported mesh import file extension: {}", extension);
		return false;
	}

	return true;
}

static const void *GetAttributeData(const tinygltf::Primitive &primitive, const std::string &attribute)
{
	if (primitive.attributes.find(attribute) != primitive.attributes.end())
	{
		auto &accessor = model.accessors[primitive.attributes.at(attribute)];
		auto &bufferView = model.bufferViews[accessor.bufferView];
		return &(model.buffers[bufferView.buffer].data[accessor.byteOffset + bufferView.byteOffset]);
	}

	return nullptr;
}

static bool UpdateIndices(const tinygltf::Primitive &primitive, const Ref<Mesh> &outMesh)
{
	// add indices
	auto &indices = model.accessors[primitive.indices];
	void *indicesData = &(model.buffers[model.bufferViews[indices.bufferView].buffer].data[indices.byteOffset + model.bufferViews[indices.bufferView].byteOffset]);

	switch (indices.componentType)
	{
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
	{
		auto buffer = static_cast<uint8_t *>(indicesData);
		outMesh->indices.insert(outMesh->indices.end(), buffer, buffer + indices.count);
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
	{
		auto buffer = static_cast<uint16_t *>(indicesData);
		outMesh->indices.insert(outMesh->indices.end(), buffer, buffer + indices.count);
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
	{
		auto buffer = static_cast<uint32_t *>(indicesData);
		outMesh->indices.insert(outMesh->indices.end(), buffer, buffer + indices.count);
		break;
	}
	default:
		return false;
	}
	return true;
}

bool MeshLibrary::Get(const std::string &path, Ref<StaticMesh> &outMesh)
{
	// TODO: get from the cache map if already loaded once

	if (!LoadFromFile(path))
		return false;
	
	// for now, we just assume the imported file is .gltf
	for (const auto &mesh : model.meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// import the indices
			if (!UpdateIndices(primitive, outMesh))
				Log::Error("GLTF Import: Indices importing error for file: {}", path);

			// create the vertices

			// get the vert count (position should always be specified)
			auto vertCount = model.accessors[primitive.attributes.at("POSITION")].count;

			// append base vertices to the mesh
			outMesh->vertices.insert(outMesh->vertices.end(), vertCount, StaticVertex());
			
			// update all attributes for each vertex

			auto data = GetAttributeData(primitive, "POSITION");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].position = buffer[i];
			}

			data = GetAttributeData(primitive, "NORMAL");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].normal = buffer[i];
			}

			data = GetAttributeData(primitive, "TANGENT");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec4 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].tangent = buffer[i];
			}

			data = GetAttributeData(primitive, "TEXCOORD_0");
			if (data)
			{
				// TODO: texcoord is not necessarily float vec2 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec2 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].texCoord = buffer[i];
			}

			data = GetAttributeData(primitive, "COLOR_0");
			if (data)
			{
				// TODO: color is not necessarily float vec3 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].color = buffer[i];
			}
		}
	}

	return true;
}

bool MeshLibrary::Get(const std::string &path, Ref<Skeleton> &outSkeleton, Ref<SkeletalMesh> &outMesh)
{
	// TODO: get from the cache map if already loaded once

	if (!LoadFromFile(path))
		return false;

	// for now, we just assume the imported file is .gltf
	for (const auto &mesh : model.meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// import the indices
			if (!UpdateIndices(primitive, outMesh))
				Log::Error("GLTF Import: Indices importing error for file: {}", path);

			// create the vertices

			// get the vert count (position should always be specified)
			auto vertCount = model.accessors[primitive.attributes.at("POSITION")].count;

			// append base vertices to the mesh
			outMesh->vertices.insert(outMesh->vertices.end(), vertCount, SkeletalVertex());

			// update all attributes for each vertex

			auto data = GetAttributeData(primitive, "POSITION");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].position = buffer[i];
			}

			data = GetAttributeData(primitive, "NORMAL");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].normal = buffer[i];
			}

			data = GetAttributeData(primitive, "TANGENT");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec4 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].tangent = buffer[i];
			}

			data = GetAttributeData(primitive, "TEXCOORD_0");
			if (data)
			{
				// TODO: texcoord is not necessarily float vec2 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec2 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].texCoord = buffer[i];
			}

			data = GetAttributeData(primitive, "COLOR_0");
			if (data)
			{
				// TODO: color is not necessarily float vec3 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].color = buffer[i];
			}

			data = GetAttributeData(primitive, "JOINTS_0");
			if (data)
			{
				// TODO: joints can also be unsigned bytes https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec<4, uint16_t> *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].bones = buffer[i];
			}

			data = GetAttributeData(primitive, "WEIGHTS_0");
			if (data)
			{
				// TODO: weights is not necessarily floats https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec4 *>(data);
				for (int i = 0; i < vertCount; i++)
					outMesh->vertices[i].weights = buffer[i];
			}
		}
	}

	return true;
}
