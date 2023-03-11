#pragma once

#include "Buffer.h"

template<typename T>
class StorageBuffer : public Buffer<T>
{
public:
	StorageBuffer(GLenum usage = GL_DYNAMIC_COPY) : Buffer<T>(usage) {}
	virtual ~StorageBuffer() = default;

public:
	void ReadData(GLintptr offsetCount, GLsizeiptr count, T *data) const
	{
		glGetNamedBufferSubData(id, offsetCount * sizeof T, count * sizeof T, data);
	}
};
