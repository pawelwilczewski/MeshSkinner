#pragma once

#include "Application/Core.h"

class ShaderLibrary
{
public:
	static Ref<Shader> Load(const std::string &displayedName, const std::string &vertexSourcePath, const std::string &fragmentSourcePath);

	static Ref<Shader> Get(const std::string &displayedName);

private:
	static Ref<Shader> FindShader(const std::string &displayedName);

private:
	static std::unordered_set<Ref<Shader>> shaders;
};
