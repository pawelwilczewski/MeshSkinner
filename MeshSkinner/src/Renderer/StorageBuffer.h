#pragma once

#include "Buffer.h"

template<typename T>
class StorageBuffer : public Buffer<T>
{
public:
	StorageBuffer(GLenum usage = GL_STATIC_DRAW) : Buffer<T>(GL_SHADER_STORAGE_BUFFER, usage) {}
	virtual ~StorageBuffer() = default;
};
