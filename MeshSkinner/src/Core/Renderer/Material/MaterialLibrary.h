#pragma once

class MaterialLibrary
{
public:
	static void Init();

public:
	static Ref<Material> GetDefault();
	static Ref<Material> GetDefaultOverlay();
};
