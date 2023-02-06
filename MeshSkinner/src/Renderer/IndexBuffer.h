#pragma once

#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(GLenum usage = GL_STATIC_DRAW);
	virtual ~IndexBuffer() = default;
};
