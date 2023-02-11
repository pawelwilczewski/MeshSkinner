#pragma once

#include "Buffer.h"

template<typename T>
class StorageBuffer : public Buffer<T>
{
public:
	StorageBuffer(GLenum usage = GL_DYNAMIC_READ) : Buffer<T>(GL_SHADER_STORAGE_BUFFER, usage) {}
	virtual ~StorageBuffer() = default;
};
