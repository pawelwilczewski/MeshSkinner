#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(GLenum usage = GL_STATIC_DRAW) : Buffer(GL_ELEMENT_ARRAY_BUFFER, usage) {}
	virtual ~IndexBuffer() = default;
};
