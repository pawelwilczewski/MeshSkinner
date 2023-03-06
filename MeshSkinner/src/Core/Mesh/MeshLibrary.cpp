#include "pch.h"
#include "MeshLibrary.h"

#include "tiny_gltf.h"

Ref<StaticMesh> MeshLibrary::GetCube()
{
	auto cubeMesh = MakeRef<StaticMesh>();
	Import("assets/models/default/cube.glb", cubeMesh);
	return cubeMesh;
}

Ref<StaticMesh> MeshLibrary::GetBone(float length)
{
	// take cube, offset the vertices accordingly and set the color to random (gradient)
	auto mesh = MakeRef<StaticMesh>();
	Import("assets/models/default/cube.glb", mesh);

	// offset to correct the origin
	for (auto &vertex : mesh->vertices)
	{
		vertex.position += 0.5;

		if (vertex.position.y > 0.5f)
			vertex.position.y += length - 1.f;
	}

	return mesh;
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

	if (!warn.empty())	Log::Warn("GLTF Import from file {}: Warning: {}", path, warn);
	if (!err.empty())	Log::Error("GLTF Import from file {}: Error: {}", path, err);
	if (!success)		Log::Error("GLTF Import: Failed to parse from file {}", path);

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

static void *GetAttributeData(const tinygltf::Primitive &primitive, const std::string &attribute)
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

bool MeshLibrary::Import(const std::string &path, Ref<StaticMesh> &outMesh)
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

bool MeshLibrary::Import(const std::string &path, Ref<SkeletalMesh> &outMesh, Ref<Bone> &outRoot)
{
	// TODO: get from the cache map if already loaded once

	if (!LoadFromFile(path))
		return false;

	// for now, we just assume the imported file is .gltf

	outMesh->skeleton = MakeRef<Skeleton>();
	// import the skeleton
	for (const auto &skin : model.skins)
	{
		// skeleton root
		outMesh->skeleton->root = -1;

		// initialize bones simple
		outMesh->skeleton->bones.resize(outMesh->skeleton->bones.size() + skin.joints.size());

		// init bones
		for (auto &bone : outMesh->skeleton->bones)
			bone.reset(new Bone());

		// get hold of inverse bind matrices data
		auto &inverseBindMatrices = model.accessors[skin.inverseBindMatrices];
		void *inverseBindMatricesData = &(model.buffers[model.bufferViews[inverseBindMatrices.bufferView].buffer].data[inverseBindMatrices.byteOffset + model.bufferViews[inverseBindMatrices.bufferView].byteOffset]);
		glm::mat4 *inverseBindMatricesMatData = static_cast<glm::mat4 *>(inverseBindMatricesData);

		for (const auto &joint : skin.joints)
		{
			// name
			outMesh->skeleton->bones[joint]->name = model.nodes[joint].name;

			// inverse bind matrix
			outMesh->skeleton->bones[joint]->inverseBindMatrix = inverseBindMatricesMatData[joint];

			// local transform
			if (model.nodes[joint].translation.size() > 0)
			{
				std::vector<float> translationData(model.nodes[joint].translation.begin(), model.nodes[joint].translation.end());
				outMesh->skeleton->bones[joint]->transform.SetPosition(glm::make_vec3(translationData.data()));
			}
			if (model.nodes[joint].rotation.size() > 0)
			{
				std::vector<float> rotationData(model.nodes[joint].rotation.begin(), model.nodes[joint].rotation.end());
				outMesh->skeleton->bones[joint]->transform.SetRotation(glm::degrees(glm::eulerAngles(glm::make_quat(rotationData.data()))));
			}
			if (model.nodes[joint].scale.size() > 0)
			{
				std::vector<float> scaleData(model.nodes[joint].scale.begin(), model.nodes[joint].scale.end());
				outMesh->skeleton->bones[joint]->transform.SetScale(glm::make_vec3(scaleData.data()));
			}

			// update children's parent index
			for (const auto &child : model.nodes[joint].children)
				outMesh->skeleton->bones[child]->SetParent(outMesh->skeleton->bones[joint]);
		}

		// update the root
		for (int i = 0; i < outMesh->skeleton->bones.size(); i++)
		{
			if (outMesh->skeleton->bones[i]->GetParent() == nullptr)
			{
				outMesh->skeleton->root = i;
				outRoot = outMesh->skeleton->bones[i];
			}
		}
	}

	// import the mesh
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
				switch (model.accessors[primitive.attributes.at("JOINTS_0")].componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				{
					auto *buffer = static_cast<const glm::vec<4, uint8_t> *>(data);
					for (int i = 0; i < vertCount; i++)
						outMesh->vertices[i].bones = buffer[i];
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				{
					auto *buffer = static_cast<const glm::vec<4, uint16_t> *>(data);
					for (int i = 0; i < vertCount; i++)
						outMesh->vertices[i].bones = buffer[i];
					break;
				}
				default:
					assert(false);
				}
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

bool MeshLibrary::Import(const std::string &path, std::vector<Animation> &outAnimations)
{
	if (!LoadFromFile(path))
		return false;

	// import the mesh
	for (const auto &animation : model.animations)
	{
		// init the animation
		auto anim = Animation(animation.name);

		for (const auto &channel : animation.channels)
		{
			// ensure the track is inserted
			auto &boneName = model.nodes[channel.target_node].name;
			if (anim.tracks.find(boneName) == anim.tracks.end())
				anim.tracks.insert({ boneName, AnimationTrack() });
			auto &track = anim.tracks[boneName];

			// interpolation mode update
			auto &interp = animation.samplers[channel.sampler].interpolation;
			if (interp == "LINEAR")
				track.interpolation = AnimationInterpolationMode::Linear;
			else if (interp == "STEP")
				track.interpolation = AnimationInterpolationMode::Step;
			else if (interp == "CUBICSPLINE")
				track.interpolation = AnimationInterpolationMode::CubicSpline;
			else
				assert(false);

			// get times bufferview
			auto &timesBufferIndex = animation.samplers[channel.sampler].input;
			auto &timesBufferAccessor = model.accessors[timesBufferIndex];
			auto &timesBufferView = model.bufferViews[timesBufferAccessor.bufferView];

			// get times data and keyframes length
			auto timesData = (float *)&(model.buffers[timesBufferView.buffer].data[timesBufferAccessor.byteOffset + timesBufferView.byteOffset]);

			// add initial keyframes with time data
			for (size_t i = 0; i < timesBufferAccessor.count; i++)
			{
				if (channel.target_path == "translation")
					track.translationKeyframes.push_back(Keyframe<glm::vec3>(timesData[i], glm::vec3(0.f)));
				else if (channel.target_path == "rotation")
					track.rotationKeyframes.push_back(Keyframe<glm::quat>(timesData[i], glm::quat()));
				else if (channel.target_path == "scale")
					track.scaleKeyframes.push_back(Keyframe<glm::vec3>(timesData[i], glm::vec3(0.f)));
				else if (channel.target_path == "weights")
					track.weightsKeyframes.push_back(Keyframe<float>(timesData[i], 0.f));
				else
					assert(false);
			}

			// get values buffer view
			auto &valuesBufferIndex = animation.samplers[channel.sampler].output;
			auto &valuesBufferAccessor = model.accessors[valuesBufferIndex];
			auto &valuesBufferView = model.bufferViews[valuesBufferAccessor.bufferView];

			// update the keyframes
			if (channel.target_path == "translation")
			{
				auto valuesData = (glm::vec3 *)&(model.buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				// update positions
				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.translationKeyframes[i].value = valuesData[i];
			}
			else if (channel.target_path == "rotation")
			{
				auto valuesData = (glm::quat *) &(model.buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				// update rotations
				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.rotationKeyframes[i].value = valuesData[i];
			}
			else if (channel.target_path == "scale")
			{
				auto valuesData = (glm::vec3 *) &(model.buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				// update scale
				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.scaleKeyframes[i].value = valuesData[i];
			}
			else if (channel.target_path == "weights")
			{
				auto valuesData = (float *) &(model.buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.weightsKeyframes[i].value = valuesData[i];
			}
			else
				assert(false);
		}

		// add the animation to the out vector
		outAnimations.push_back(anim);
	}

	return false;
}

void MeshLibrary::ExportUpdated(const std::string &source, const std::string &target, const Ref<SkeletalMesh> &inMesh)
{
	LoadGLTF(source);

	// import the mesh
	for (const auto &mesh : model.meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// update joints and weights attributes for each vertex
			auto vertCount = inMesh->GetVerticesLength();
			auto data = GetAttributeData(primitive, "JOINTS_0");
			if (data)
			{
				switch (model.accessors[primitive.attributes.at("JOINTS_0")].componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				{
					auto *buffer = static_cast<glm::vec<4, uint8_t> *>(data);
					for (int i = 0; i < vertCount; i++)
						buffer[i] = static_cast<glm::vec<4, uint8_t>>(inMesh->vertices[i].bones);
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				{
					auto *buffer = static_cast<glm::vec<4, uint16_t> *>(data);
					for (int i = 0; i < vertCount; i++)
						buffer[i] = inMesh->vertices[i].bones;
					break;
				}
				default:
					assert(false);
				}
			}

			data = GetAttributeData(primitive, "WEIGHTS_0");
			if (data)
			{
				// TODO: weights is not necessarily floats https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<glm::vec4 *>(data);
				for (int i = 0; i < vertCount; i++)
					buffer[i] = inMesh->vertices[i].weights;
			}
		}
	}

	loader.WriteGltfSceneToFile(&model, target, true, true, true, FileUtils::FileExtension(target) == ".glb");
}
