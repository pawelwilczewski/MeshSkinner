#include "pch.h"
#include "MeshLibrary.h"

#include "tiny_gltf.h"

Ref<StaticMeshComponent> MeshLibrary::GetCube()
{
	return Import("assets/models/default/cube.glb", false);
}

Ref<StaticMeshComponent> MeshLibrary::GetBone(float length)
{
	// take cube, offset the vertices accordingly and set the color to random (gradient)
	auto mesh = Import("assets/models/default/cube.glb", false);

	// offset to correct the origin
	for (auto &vertex : mesh->vertices)
	{
		vertex.position.y += 0.5f;

		if (vertex.position.y > 0.6f)
			vertex.position.y += length - 1.f;
	}

	return mesh;
}

static Unique<tinygltf::Model> model = nullptr;
static Unique<tinygltf::TinyGLTF> loader = nullptr;
static std::string err;
static std::string warn;

static bool LoadGLTF(const std::string &path)
{
	loader = MakeUnique<tinygltf::TinyGLTF>();
	model = MakeUnique<tinygltf::Model>();

	bool success = false;
	auto extension = std::filesystem::path(path).extension();

	if (extension == ".gltf")		success = loader->LoadASCIIFromFile(model.get(), &err, &warn, path);
	else if (extension == ".glb")	success = loader->LoadBinaryFromFile(model.get(), &err, &warn, path);

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
		auto &accessor = model->accessors[primitive.attributes.at(attribute)];
		auto &bufferView = model->bufferViews[accessor.bufferView];
		return &(model->buffers[bufferView.buffer].data[accessor.byteOffset + bufferView.byteOffset]);
	}

	return nullptr;
}

static bool UpdateIndices(const tinygltf::Primitive &primitive, const Ref<MeshComponent> &outMesh)
{
	// add indices
	auto &indices = model->accessors[primitive.indices];
	void *indicesData = &(model->buffers[model->bufferViews[indices.bufferView].buffer].data[indices.byteOffset + model->bufferViews[indices.bufferView].byteOffset]);

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
		assert(false);
	}
	return true;
}

Ref<StaticMeshComponent> MeshLibrary::Import(const std::string &path, bool triggerEvent)
{
	if (!LoadFromFile(path))
		return nullptr;
	
	auto resultMesh = MakeRef<StaticMeshComponent>();

	// for now, we just assume the imported file is .gltf
	for (const auto &mesh : model->meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// import the indices
			if (!UpdateIndices(primitive, resultMesh))
				Log::Error("GLTF Import: Indices importing error for file: {}", path);

			// create the vertices

			// get the vert count (position should always be specified)
			auto vertCount = model->accessors[primitive.attributes.at("POSITION")].count;

			// append base vertices to the mesh
			resultMesh->vertices.insert(resultMesh->vertices.end(), vertCount, StaticVertex());
			
			// update all attributes for each vertex

			auto data = GetAttributeData(primitive, "POSITION");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].position = buffer[i];
			}

			data = GetAttributeData(primitive, "NORMAL");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].normal = buffer[i];
			}

			data = GetAttributeData(primitive, "TANGENT");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec4 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].tangent = buffer[i];
			}

			data = GetAttributeData(primitive, "TEXCOORD_0");
			if (data)
			{
				// TODO: texcoord is not necessarily float vec2 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec2 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].texCoord = buffer[i];
			}

			data = GetAttributeData(primitive, "COLOR_0");
			if (data)
			{
				// TODO: color is not necessarily float vec3 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].color = buffer[i];
			}
		}
	}

	if (triggerEvent)
		onStaticMeshImported.Invoke(resultMesh.get());

	return resultMesh;
}

Ref<SkeletalMeshComponent> MeshLibrary::Import(const std::string &path, Scene *scene, Entity *entity)
{
	if (!entity)
	{
		Log::Error("Can't import into invalid entity");
		return nullptr;
	}

	if (!LoadFromFile(path))
		return nullptr;

	// for now, we just assume the imported file is .gltf
	auto resultMesh = MakeRef<SkeletalMeshComponent>();

	// import the skeleton
	for (const auto &skin : model->skins)
	{
		// skeleton root
		resultMesh->skeleton->root = -1;

		// initialize bones simple
		resultMesh->skeleton->bones.resize(resultMesh->skeleton->bones.size() + skin.joints.size());

		// init bones
		for (auto &bone : resultMesh->skeleton->bones)
			bone = static_cast<Bone *>(scene->CreateEntity(new Bone()));

		// get hold of inverse bind matrices data
		auto &inverseBindMatricesAccessor = model->accessors[skin.inverseBindMatrices];
		auto &inverseBindMatricesBufferView = model->bufferViews[inverseBindMatricesAccessor.bufferView];
		void *inverseBindMatricesData = &(model->buffers[inverseBindMatricesBufferView.buffer].data[inverseBindMatricesAccessor.byteOffset + inverseBindMatricesBufferView.byteOffset]);
		glm::mat4 *inverseBindMatricesMatData = static_cast<glm::mat4 *>(inverseBindMatricesData);

		int i = 0;
		for (const auto &joint : skin.joints)
		{
			auto &refJoint = model->nodes[joint];
			auto &bone = resultMesh->skeleton->bones[i];

			// name
			bone->name = refJoint.name;

			// inverse bind matrix
			bone->inverseBindMatrix = inverseBindMatricesMatData[i];

			// local transform
			if (refJoint.translation.size() > 0)
			{
				std::vector<float> translationData(refJoint.translation.begin(), refJoint.translation.end());
				bone->transform.SetPosition(glm::make_vec3(translationData.data()));
			}
			if (refJoint.rotation.size() > 0)
			{
				std::vector<float> rotationData(refJoint.rotation.begin(), refJoint.rotation.end());
				bone->transform.SetRotation(glm::degrees(glm::eulerAngles(glm::make_quat(rotationData.data()))));
			}
			if (refJoint.scale.size() > 0)
			{
				std::vector<float> scaleData(refJoint.scale.begin(), refJoint.scale.end());
				bone->transform.SetScale(glm::make_vec3(scaleData.data()));
			}

			// update children's parent index
			for (const auto &child : refJoint.children)
			{
				// child refers to node index overall, not an index in the joints array
				// so just find the index of the child in the joints array and use it here
				auto it = std::find(skin.joints.begin(), skin.joints.end(), child);
				if (it != skin.joints.end())
				{
					auto index = it - skin.joints.begin();
					resultMesh->skeleton->bones[index]->SetParent(bone);
				}
			}

			i++;
		}

		// update the root
		for (int i = 0; i < resultMesh->skeleton->bones.size(); i++)
		{
			if (resultMesh->skeleton->bones[i]->GetParent() == scene->GetRoot())
			{
				resultMesh->skeleton->root = i;
				resultMesh->skeleton->bones[i]->SetParent(entity);
			}
		}
	}

	// import the mesh
	for (const auto &mesh : model->meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// import the indices
			if (!UpdateIndices(primitive, resultMesh))
				Log::Error("GLTF Import: Indices importing error for file: {}", path);

			// create the vertices

			// get the vert count (position should always be specified)
			auto vertCount = model->accessors[primitive.attributes.at("POSITION")].count;

			// append base vertices to the mesh
			resultMesh->vertices.insert(resultMesh->vertices.end(), vertCount, SkeletalVertex());

			// update all attributes for each vertex

			auto data = GetAttributeData(primitive, "POSITION");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].position = buffer[i];
			}

			data = GetAttributeData(primitive, "NORMAL");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].normal = buffer[i];
			}

			data = GetAttributeData(primitive, "TANGENT");
			if (data)
			{
				auto *buffer = static_cast<const glm::vec4 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].tangent = buffer[i];
			}

			data = GetAttributeData(primitive, "TEXCOORD_0");
			if (data)
			{
				// TODO: texcoord is not necessarily float vec2 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec2 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].texCoord = buffer[i];
			}

			data = GetAttributeData(primitive, "COLOR_0");
			if (data)
			{
				// TODO: color is not necessarily float vec3 https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#meshes
				auto *buffer = static_cast<const glm::vec3 *>(data);
				for (int i = 0; i < vertCount; i++)
					resultMesh->vertices[i].color = buffer[i];
			}

			data = GetAttributeData(primitive, "JOINTS_0");
			if (data)
			{
				switch (model->accessors[primitive.attributes.at("JOINTS_0")].componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				{
					auto *buffer = static_cast<const glm::vec<4, uint8_t> *>(data);
					for (int i = 0; i < vertCount; i++)
						resultMesh->vertices[i].bones = buffer[i];
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				{
					auto *buffer = static_cast<const glm::vec<4, uint16_t> *>(data);
					for (int i = 0; i < vertCount; i++)
						resultMesh->vertices[i].bones = buffer[i];
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
					resultMesh->vertices[i].weights = buffer[i];
			}
		}
	}

	onSkeletalMeshImported.Invoke(resultMesh.get());
	return resultMesh;
}

bool MeshLibrary::Import(const std::string &path, std::vector<Animation> &outAnimations)
{
	if (!LoadFromFile(path))
		return false;

	// import the mesh
	for (const auto &animation : model->animations)
	{
		// init the animation
		auto anim = Animation(animation.name);

		for (const auto &channel : animation.channels)
		{
			// ensure the track is inserted
			auto &boneName = model->nodes[channel.target_node].name;
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
			auto &timesBufferAccessor = model->accessors[timesBufferIndex];
			auto &timesBufferView = model->bufferViews[timesBufferAccessor.bufferView];

			// get times data and keyframes length
			auto timesData = (float *)&(model->buffers[timesBufferView.buffer].data[timesBufferAccessor.byteOffset + timesBufferView.byteOffset]);

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
			auto &valuesBufferAccessor = model->accessors[valuesBufferIndex];
			auto &valuesBufferView = model->bufferViews[valuesBufferAccessor.bufferView];

			// update the keyframes
			if (channel.target_path == "translation")
			{
				auto valuesData = (glm::vec3 *)&(model->buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				// update positions
				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.translationKeyframes[i].value = valuesData[i];
			}
			else if (channel.target_path == "rotation")
			{
				auto valuesData = (glm::quat *) &(model->buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				// update rotations
				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.rotationKeyframes[i].value = valuesData[i];
			}
			else if (channel.target_path == "scale")
			{
				auto valuesData = (glm::vec3 *) &(model->buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				// update scale
				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.scaleKeyframes[i].value = valuesData[i];
			}
			else if (channel.target_path == "weights")
			{
				auto valuesData = (float *) &(model->buffers[valuesBufferView.buffer].data[valuesBufferAccessor.byteOffset + valuesBufferView.byteOffset]);

				for (size_t i = 0; i < valuesBufferAccessor.count; i++)
					track.weightsKeyframes[i].value = valuesData[i];
			}
			else
				assert(false);
		}

		// update the duration
		for (const auto &track : anim.tracks)
		{
			if (track.second.translationKeyframes.size() > 0)
				anim.duration = glm::max(anim.duration, track.second.translationKeyframes.back().time);
			if (track.second.rotationKeyframes.size() > 0)
				anim.duration = glm::max(anim.duration, track.second.rotationKeyframes.back().time);
			if (track.second.scaleKeyframes.size() > 0)
				anim.duration = glm::max(anim.duration, track.second.scaleKeyframes.back().time);
			if (track.second.weightsKeyframes.size() > 0)
				anim.duration = glm::max(anim.duration, track.second.weightsKeyframes.back().time);
		}

		// add the animation to the out vector
		outAnimations.push_back(anim);
	}

	return false;
}

void MeshLibrary::ExportUpdated(const std::string &source, const std::string &target, const SkeletalMeshComponent *inMesh)
{
	if (!inMesh) return;

	LoadGLTF(source);

	// import the mesh
	for (const auto &mesh : model->meshes)
	{
		for (const auto &primitive : mesh.primitives)
		{
			// update joints and weights attributes for each vertex
			auto vertCount = inMesh->GetVerticesLength();
			auto data = GetAttributeData(primitive, "JOINTS_0");
			if (data)
			{
				switch (model->accessors[primitive.attributes.at("JOINTS_0")].componentType)
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

	loader->WriteGltfSceneToFile(model.get(), target, true, true, true, FileUtils::FileExtension(target) == ".glb");
}

void MeshLibrary::OnStaticMeshImportedSubscribe(const CallbackRef<StaticMeshComponent *> &callback)
{
	onStaticMeshImported.Subscribe(callback);
}

void MeshLibrary::OnStaticMeshImportedUnsubscribe(const CallbackRef<StaticMeshComponent *> &callback)
{
	onStaticMeshImported.Unsubscribe(callback);
}

void MeshLibrary::OnSkeletalMeshImportedSubscribe(const CallbackRef<SkeletalMeshComponent *> &callback)
{
	onSkeletalMeshImported.Subscribe(callback);
}

void MeshLibrary::OnSkeletalMeshImportedUnsubscribe(const CallbackRef<SkeletalMeshComponent *> &callback)
{
	onSkeletalMeshImported.Unsubscribe(callback);
}
