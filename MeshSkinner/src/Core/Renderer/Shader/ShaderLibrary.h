#pragma once

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
	static std::unordered_set<Ref<Shader>> shaders;
};
