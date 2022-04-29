#include "enginepch.h"
#include "OpenGLVertexBuffer.h"
#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t numOfVertices)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertices, vertices, GL_STATIC_DRAW);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(Vertex* vertices, uint32_t numOfVertices)
		: m_layout(vertices->GetBufferLayout())
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfVertices, vertices, GL_STATIC_DRAW);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}
}
