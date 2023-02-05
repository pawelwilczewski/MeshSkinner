#pragma once

#include "glad/glad.h"

class Buffer
{
protected:
	Buffer(GLenum type, GLenum usage = GL_STATIC_DRAW);
	virtual ~Buffer();

public:
	void Bind() const;
	void Unbind() const;
	void SetData(const void *data, GLuint size, GLuint offsetBytes = 0);

public:
	GLuint GetId() const { return id; }
	GLuint GetSize() const { return size; }

protected:
	GLuint id;
	GLuint size;
	GLenum type;
	GLenum usage;
};
