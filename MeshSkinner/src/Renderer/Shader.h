#pragma once

class Shader
{
public:
	Shader(const std::string &displayedName, const std::string &vertexSourcePath, const std::string &fragmentSourcePath);
	virtual ~Shader();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual void UploadUniformInt(const std::string &name, const int &val);
	virtual void UploadUniformFloat(const std::string &name, const float &val);
	virtual void UploadUniformFloat2(const std::string &name, const glm::vec2 &val);
	virtual void UploadUniformFloat3(const std::string &name, const glm::vec3 &val);
	virtual void UploadUniformFloat4(const std::string &name, const glm::vec4 &val);
	virtual void UploadUniformMat4(const std::string &name, const glm::mat4 &val);
	virtual void UploadUniformIntArray(const std::string &name, const int *val, uint32_t count);

	virtual void SetupStorageBuffer(const std::string &name, int slot, uint32_t ssbo);

private:
	void Compile(const std::unordered_map<GLenum, std::string> &source);
	std::unordered_map<GLenum, std::string> ExtractShadersSource(const std::string &filePath, const std::string &fileSource);
	GLint GetUniformLocation(const std::string &name);

protected:
	std::string displayedName;

private:
	GLuint id;
	std::unordered_map<std::string, GLint> uniformLocations;
};
