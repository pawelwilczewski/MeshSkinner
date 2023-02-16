#pragma once

#include "Core/Renderer/Shader/Shader.h"

struct Material
{
	Material(Ref<Shader> shader);

	Ref<Shader> shader;
};

struct MaterialGPU
{
	MaterialGPU(const Material &material);
};
