#pragma once

#include "Shader.h"

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
