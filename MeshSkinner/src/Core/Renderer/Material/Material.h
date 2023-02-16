#pragma once

#include "Core/Renderer/Shader/Shader.h"

struct Material
{
	Ref<Shader> shader;
};

struct MaterialGPU
{
	MaterialGPU(const Material &material)
	{

	}
};
