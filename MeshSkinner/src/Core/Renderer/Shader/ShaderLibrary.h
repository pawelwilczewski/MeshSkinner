#pragma once

#include "Application/Core.h"

class ShaderLibrary
{
public:
	static void Init();

public:
	static Ref<Shader> Load(const std::string &displayedName, const std::string &vertexSourcePath, const std::string &fragmentSourcePath, uint16_t depth = 0);

	static Ref<Shader> Get(const std::string &displayedName);

public:
	static Ref<Shader> GetDefault();
	static Ref<Shader> GetDefaultOverlay();

private:
	static Ref<Shader> FindShader(const std::string &displayedName);

private:
	// TODO: URGENT use map<string, ref<shader>> here instead (string key is displayedName)
	static std::unordered_set<Ref<Shader>> shaders;
};
