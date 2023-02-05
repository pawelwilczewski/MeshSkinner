#pragma once

#include "glad/glad.h"

class Buffer
{
public:
	Buffer() = default;
	virtual ~Buffer() = default;

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetData(const void *data, GLuint size, GLuint offsetBytes = 0) = 0;
	virtual void Resize(GLuint newSize) = 0;

public:
	GLuint GetId() const { return id; }
	GLuint GetSize() const { return size; }

protected:
	GLuint id;
	GLuint size;
};
