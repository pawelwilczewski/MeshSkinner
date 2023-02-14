#include "pch.h"
#include "Buffer.h"

GenericBuffer::GenericBuffer(GLenum usage) : usage(usage)
{
	glCreateBuffers(1, &id);
}

GenericBuffer::~GenericBuffer()
{
	glDeleteBuffers(1, &id);
}

void GenericBuffer::SetData(const void *data, GLuint sizeBytes, GLuint sizeOffset)
{
	Log::Info("Set data {}, size offset: {}, size: {}", data, sizeOffset, sizeBytes);

	// automatically resize to twice the size if exceeded limit - like in the vector class etc.
	auto fullSize = sizeOffset + sizeBytes;

	// don't do that if length is 0 - only just initialized
	if (this->sizeBytes == 0)
	{
		this->capacityBytes = fullSize;
		glNamedBufferData(id, this->capacityBytes, nullptr, usage);
	}
	else if (fullSize > this->capacityBytes)
	{
		// cache the buffer's data so we don't lose the data currently stored
		GLuint copiedBuffer;
		glCreateBuffers(1, &copiedBuffer);
		glNamedBufferData(copiedBuffer, this->sizeBytes, nullptr, GL_STATIC_COPY);
		glCopyNamedBufferSubData(id, copiedBuffer, 0, 0, this->sizeBytes);

		// perform the resizing
		this->capacityBytes = fullSize * 2;
		glNamedBufferData(id, this->capacityBytes, nullptr, usage);

		// restore the initial data
		glCopyNamedBufferSubData(copiedBuffer, id, 0, 0, this->sizeBytes);

		// delete the temp buffer
		glDeleteBuffers(1, &copiedBuffer);
	}

	// append the data at the end - sufficient length ensured
	glNamedBufferSubData(id, sizeOffset, sizeBytes, data);

	// update length if just extended
	if (fullSize > this->sizeBytes)
		this->sizeBytes = fullSize;
}

void GenericBuffer::Clear()
{
	// no need to do unnecessary gl calls, just change the size
	this->sizeBytes = 0;
}
