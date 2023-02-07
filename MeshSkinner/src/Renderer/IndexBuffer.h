#pragma once

#include "Buffer.h"

template<typename T>
class IndexBuffer : public Buffer<T>
{
public:
	IndexBuffer(GLenum usage = GL_STATIC_DRAW) : Buffer<T>(GL_ELEMENT_ARRAY_BUFFER, usage) {}
	virtual ~IndexBuffer() = default;
};
