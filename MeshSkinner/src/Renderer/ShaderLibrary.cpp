#include "pch.h"
#include "ShaderLibrary.h"

std::unordered_set<Ref<Shader>> ShaderLibrary::shaders;

Ref<Shader> ShaderLibrary::FindShader(const std::string &displayedName)
{
	for (const auto &shader : shaders)
		if ((*shader.get()) == displayedName)
			return shader;

	return nullptr;
}

Ref<Shader> ShaderLibrary::Load(const std::string &displayedName, const std::string &vertexSourcePath, const std::string &fragmentSourcePath)
{
	auto shader = FindShader(displayedName);

	if (shader)
	{
		Log::Warn("Attempting to load a shader which already exists \"{}\"!", displayedName);
		return shader;
	}

	shader = MakeRef<Shader>(displayedName, vertexSourcePath, fragmentSourcePath);
	shaders.insert(shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string &displayedName)
{
	auto result = FindShader(displayedName);

	if (!result)
		Log::Critical("The shader \"{}\" not loaded: get attempt failure!", displayedName);

	return result;
}
