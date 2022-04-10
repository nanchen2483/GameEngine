#include "enginepch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t numOfIndices)
		: m_numOfIndices(numOfIndices)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_rendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
