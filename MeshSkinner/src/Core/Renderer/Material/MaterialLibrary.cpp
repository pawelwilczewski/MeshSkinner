#include "pch.h"
#include "MaterialLibrary.h"

static Ref<Material> defaultMaterial;
static Ref<Material> defaultOverlayMaterial;

void MaterialLibrary::Init()
{
	defaultMaterial = MakeRef<Material>(ShaderLibrary::GetDefault());
	defaultOverlayMaterial = MakeRef<Material>(ShaderLibrary::GetDefaultOverlay());
}

Ref<Material> MaterialLibrary::GetDefault()
{
	return defaultMaterial;
}

Ref<Material> MaterialLibrary::GetDefaultOverlay()
{
	return defaultOverlayMaterial;
}
