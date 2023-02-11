#pragma once

#include "glad/glad.h"

class GenericBuffer
{
public:
	GenericBuffer(GLenum usage = GL_STATIC_DRAW);
	virtual ~GenericBuffer();

public:
	virtual void SetData(const void *data, GLuint sizeBytes, GLuint sizeOffset = 0);
	virtual void Clear();

public:
	GLuint GetID() const { return id; }
	GLuint GetSizeBytes() const { return sizeBytes; }
	GLuint GetCapacityBytes() const { return capacityBytes; }

protected:
	GLuint id;
	GLuint capacityBytes = 0;
	GLuint sizeBytes = 0;
	GLenum usage;
};

template<typename T>
class Buffer : public GenericBuffer
{
public:
	Buffer(GLenum usage = GL_STATIC_DRAW) : GenericBuffer(usage) {}
	virtual ~Buffer() = default;

public:
	virtual void SetData(const T *data, GLuint length, GLuint lengthOffset = 0)
	{
		GenericBuffer::SetData(data, length * sizeof T, lengthOffset * sizeof T);
	}

public:
	GLuint GetLength() const { return sizeBytes / sizeof T; }
	GLuint GetCapacity() const { return capacityBytes / sizeof T; }
};
