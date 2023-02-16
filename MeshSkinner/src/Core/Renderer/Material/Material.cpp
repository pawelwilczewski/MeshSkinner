#include "pch.h"
#include "Material.h"

Material::Material(Ref<Shader> shader) : shader(shader)
{

}

MaterialGPU::MaterialGPU(const Material &material)
{

}
