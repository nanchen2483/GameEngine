#include "enginepch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t index, const BufferLayout layout)
		: m_layout(layout)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
		glBufferData(GL_UNIFORM_BUFFER, m_layout.GetStride(), nullptr, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, index, m_rendererId, 0, m_layout.GetStride());
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_rendererId);
	}

	void OpenGLUniformBuffer::Bind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
	}

	void OpenGLUniformBuffer::Unbind() const
	{
		ENGINE_PROFILE_FUNCTION();

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::SetData(const std::vector<const void*>& data)
	{
		ENGINE_PROFILE_FUNCTION();

		ENGINE_CORE_ASSERT(m_layout.GetNumOfElements() == data.size(), "Incorrect number of data");
		for (uint32_t i = 0; i < m_layout.GetNumOfElements(); i++)
		{
			const BufferElement& element = m_layout[i];
			glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
			glBufferSubData(GL_UNIFORM_BUFFER, element.offset, element.size, data[i]);
		}
	}
}
