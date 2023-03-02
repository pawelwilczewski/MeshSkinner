#pragma once

// standard libraries
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <functional>

// dependencies
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "imgui.h"

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
#include "Application/Core.h"
#include "Application/Log.h"
#include "Application/Input.h"
#include "Application/Time.h"
#include "Application/Window.h"
#include "Application/UserInterface.h"
#include "Application/Scene.h"
#include "Core/Entity/Entity.h"
#include "Core/Entity/EntityComponent.h"
#include "Core/Entity/Transform.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader/Shader.h"
#include "Core/Renderer/Shader/ShaderLibrary.h"
#include "Core/Renderer/Material/Material.h"
#include "Core/Renderer/Material/MaterialLibrary.h"
#include "Core/Mesh/Mesh.h"
#include "Core/Mesh/Skeleton.h"
#include "Core/Mesh/MeshLibrary.h"

#include "Utils/FileUtils.h"
#include "Utils/MathUtils.h"

// platform-variable
#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif
