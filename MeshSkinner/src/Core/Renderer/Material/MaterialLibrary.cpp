#include "pch.h"
#include "MaterialLibrary.h"

static Ref<Material> defaultMaterial;

void MaterialLibrary::Init()
{
	defaultMaterial = MakeRef<Material>(ShaderLibrary::GetDefault());
}

Ref<Material> MaterialLibrary::GetDefault()
{
	return defaultMaterial;
}
