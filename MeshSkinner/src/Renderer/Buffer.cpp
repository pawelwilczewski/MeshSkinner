#include "pch.h"
#include "Buffer.h"

Buffer::Buffer(GLenum type, GLenum usage) : size(0), type(type), usage(usage)
{
	glGenBuffers(1, &id);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &id);
}

void Buffer::Bind() const
{
	glBindBuffer(type, id);
}

void Buffer::Unbind() const
{
	glBindBuffer(type, 0);
}

void Buffer::SetData(const void *data, GLuint size, GLuint offsetBytes)
{
	glBindBuffer(type, id);

	// automatically resize to twice the size if exceeded limit - like in the vector class etc.
	// don't do that if size is 0 - only just initialized
	auto fullSize = offsetBytes + size;
	if (this->size > 0 && fullSize > capacity)
	{
		// cache the buffer's data so we don't lose the data currently stored
		GLuint copiedBuffer;
		glGenBuffers(1, &copiedBuffer);
		glCopyBufferSubData(id, copiedBuffer, 0, 0, size);

		// perform the resizing
		capacity = fullSize * 2;
		glBufferData(type, capacity, nullptr, usage);

		// restore the initial data
		glCopyBufferSubData(copiedBuffer, id, 0, 0, this->size);
		this->size = fullSize;

		// delete the temp buffer
		glDeleteBuffers(1, &copiedBuffer);
	}

	// append the data at the end - sufficient size ensured
	glBufferSubData(type, offsetBytes, size, data);
}
