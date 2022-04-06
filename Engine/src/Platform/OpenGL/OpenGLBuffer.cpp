#include "enginepch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	/// <summary>
	/// Vertex buffer
	/// </summary>
	/// <param name="size"></param>
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t numOfVertices)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertices, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex* vertices, uint32_t numOfVertices)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfVertices, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		ENGINE_PROFILE_FUNCTION();
		
		glDeleteBuffers(1, &m_rendererId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	/// <summary>
	/// Index buffer
	/// </summary>
	/// <param name="indices"></param>
	/// <param name="size"></param>
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
