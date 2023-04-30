#pragma once

// memory
#include <memory>

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ...Args>
constexpr Unique<T> MakeUnique(Args &&...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ...Args>
constexpr Ref<T> MakeRef(Args &&...args) { return std::make_shared<T>(std::forward<Args>(args)...); }

template<typename T>
using Weak = std::weak_ptr<T>;

// standard libraries
#include <iostream>
#include <utility>
#include <algorithm>
#include <functional>
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <filesystem>
#include <functional>

// dependencies
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

// glm
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat2x2.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 1st party code
#include "Application/Application.h"
#include "Application/Log.h"
#include "Application/Input.h"
#include "Application/Time.h"
#include "Application/Window.h"
#include "Application/UserInterface.h"
#include "Core/Entity/Transform.h"
#include "Core/Entity/Scene.h"
#include "Core/Entity/Entity.h"
#include "Core/Entity/EntityComponent.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader/Shader.h"
#include "Core/Renderer/Shader/ShaderLibrary.h"
#include "Core/Renderer/Material/Material.h"
#include "Core/Renderer/Material/MaterialLibrary.h"
#include "Core/Mesh/MeshComponent.h"
#include "Core/Mesh/StaticMeshComponent.h"
#include "Core/Mesh/SkeletalMeshComponent.h"
#include "Core/Mesh/Skeleton.h"
#include "Core/Animation/Animation.h"
#include "Core/Mesh/MeshLibrary.h"

// utils
#include "Core/Utils/FileUtils.h"
#include "Core/Utils/MathUtils.h"
