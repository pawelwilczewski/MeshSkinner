#include "pch.h"
#include "Shader.h"

static void ReplaceFirst(std::string &s, std::string const &toReplace, std::string const &replaceWith)
{
	std::size_t pos = s.find(toReplace);
	if (pos == std::string::npos) return;
	s.replace(pos, toReplace.length(), replaceWith);
}

static std::string AddLineNumbers(const std::string &source)
{
	std::string result = "1: " + source;

	int number = 2;
	std::size_t lastPos = 0;

	std::size_t lastLength = 0;
	while (true)
	{
		std::size_t pos = result.find('\n', lastPos + 1);
		if (pos == std::string::npos) break;

		auto added = "\n" + std::to_string(number++) + ": ";
		result.replace(pos, 1, added);
		lastPos = pos + added.length();
	}

	return result;
}

// first lambda argument is the text after the token between the double quotation marks "
// second argument is the following text until the next token of the same arg occurs
// returns the substring before the first token
static std::string ParseToken(const std::string &token, const std::string &source, std::function<void(const std::string &, const std::string &)> parseCallback)
{
	auto start = source.find(token);

	std::string before;
	if (start == std::string::npos)
		before = source;
	else
		before = source.substr(0, start);

	while (source.length() > start)
	{
		// determine the arg
		auto firstQuotation = source.find('"', start);
		auto secondQuotation = source.find('"', firstQuotation + 1);

		assert(firstQuotation != std::string::npos && secondQuotation != std::string::npos);//, "Shader parsing error (probably missing \")! {0}", source);
		std::string arg = source.substr(firstQuotation + 1, secondQuotation - firstQuotation - 1);

		// finished? the loop is gonna terminate if end is npos
		auto end = source.find(token, start + token.length());
		parseCallback(arg, source.substr(secondQuotation + 1, end - secondQuotation - 1));
		start = end;
	}

	return before;
}

// can be nested includes (when #include appears in an included file)
// the limitation is you can't yet include the same file twice in one file, regardless of which shader type the include directive is in
// also, DON'T have #include in comments (TODO?)
static std::string ResolveIncludes(const std::string &filePath, const std::string &fileSource)
{
	// TODO: important: since we have multiple shaders in one file, we need to append the shader type to the file
	//  so we can include the same file multiple times as long as it is in different shader types
	std::vector <std::string> filesIncluded = { filePath };
	std::string result = fileSource;
	std::string toParse = fileSource;

	while (toParse.find("#include") != std::string::npos)
	{
		result = "";
		std::string before = ParseToken("#include", toParse, [&](const std::string &arg, const std::string &next) {
			auto argFileStr = FileUtils::FileParentPath(filePath) + "/" + arg;

		// skip if file already included
		if (std::find(filesIncluded.begin(), filesIncluded.end(), argFileStr) != filesIncluded.end())
		{
			result += next;
			return;
		}
		else
			filesIncluded.push_back(argFileStr);

		result += FileUtils::ReadFile(argFileStr) + next;
		//finalSrc += "\r\n" + next; // just to be safe, ensure the newline as well
			});
		result = before + result;
		toParse = result;
	}
	return result;
}

Shader::Shader(const std::string &displayedName, const std::string &vertexSourcePath, const std::string &fragmentSourcePath) : displayedName(displayedName)
{
	std::unordered_map<GLenum, std::string> data;
	data[GL_VERTEX_SHADER] = ResolveIncludes(vertexSourcePath, FileUtils::ReadFile(vertexSourcePath));
	data[GL_FRAGMENT_SHADER] = ResolveIncludes(fragmentSourcePath, FileUtils::ReadFile(fragmentSourcePath));
	Compile(data);

	this->vertexPath = vertexSourcePath;
	this->fragmentPath = fragmentSourcePath;
}

Shader::~Shader()
{
	glDeleteProgram(id);
}

void Shader::Compile(const std::unordered_map<GLenum, std::string> &data)
{
	// Get a program object.
	id = glCreateProgram();
	GLuint program = id;

	std::vector<GLuint> shaders;
	shaders.reserve(data.size());
	for (const auto &s : data)
	{
		auto &[type, source] = s;

		// Create an empty vertex Shader handle
		GLuint shader = glCreateShader(type);

		// Send the Shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar *s_ = (const GLchar *)source.c_str();
		glShaderSource(shader, 1, &s_, 0);

		// Compile the Shader
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the Shader anymore.
			glDeleteShader(shader);

			Log::Critical("Error during shader compilation: {}\n", infoLog.data());
			Log::Critical("Shader source:\n{}", AddLineNumbers(s_));
			return;
		}

		shaders.push_back(shader);
		glAttachShader(program, shader);
	}

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);

		// Don't leak shaders either.
		for (const auto &shader : shaders)
			glDeleteShader(shader);

		Log::Critical("Error during shader linking: {}\n", infoLog.data());
		return;
	}

	// Always detach shaders after a successful link.
	for (const auto &shader : shaders)
		glDetachShader(program, shader);
}

GLint Shader::GetUniformLocation(const std::string &name)
{
	if (uniformLocations.find(name) != uniformLocations.end())
		return uniformLocations[name];

	GLint result = glGetUniformLocation(id, name.c_str());
	uniformLocations[name] = result;
	return result;
}

bool Shader::operator==(const Shader &other)
{
	return displayedName == other.displayedName || vertexPath == other.vertexPath && fragmentPath == other.fragmentPath;
}

bool Shader::operator==(const std::string &other)
{
	return displayedName == other;
}

void Shader::Bind() const
{
	glUseProgram(id);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::UploadUniformInt(const std::string &name, const int &val)
{
	glUniform1i(GetUniformLocation(name), val);
}

void Shader::UploadUniformFloat(const std::string &name, const float &val)
{
	glUniform1f(GetUniformLocation(name), val);
}

void Shader::UploadUniformFloat2(const std::string &name, const glm::vec2 &val)
{
	glUniform2f(GetUniformLocation(name), val.x, val.y);
}

void Shader::UploadUniformFloat3(const std::string &name, const glm::vec3 &val)
{
	glUniform3f(GetUniformLocation(name), val.x, val.y, val.z);
}

void Shader::UploadUniformFloat4(const std::string &name, const glm::vec4 &val)
{
	glUniform4f(GetUniformLocation(name), val.x, val.y, val.z, val.w);
}

void Shader::UploadUniformMat4(const std::string &name, const glm::mat4 &val)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::UploadUniformIntArray(const std::string &name, const int *val, uint32_t count)
{
	glUniform1iv(GetUniformLocation(name), count, val);
}

void Shader::SetupStorageBuffer(const std::string &name, int slot, uint32_t ssbo)
{
	GLuint block = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, name.c_str());
	glShaderStorageBlockBinding(id, block, slot);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, ssbo);
}
