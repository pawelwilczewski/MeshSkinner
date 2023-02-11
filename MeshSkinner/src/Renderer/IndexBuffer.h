#pragma once

#include "Buffer.h"

template<typename T>
class IndexBuffer : public Buffer<T>
{
public:
	IndexBuffer(GLenum usage = GL_STATIC_DRAW) : Buffer<T>(usage) {}
	virtual ~IndexBuffer() = default;
};
