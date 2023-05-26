#include "enginepch.h"
#include "OpenGLUniformBuffer.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <glad/glad.h>

namespace Engine
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t index, const BufferLayout layout, uint32_t numOfUniforms)
		: m_layout(layout)
	{
		ENGINE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
		glBufferData(GL_UNIFORM_BUFFER, m_layout.GetStride() * numOfUniforms, nullptr, GL_STATIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, index, m_rendererId, 0, m_layout.GetStride() * numOfUniforms);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		ENGINE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_rendererId);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
		
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
		for (uint32_t i = 0; i < m_layout.GetNumOfElements(); ++i)
		{
			const BufferElement& element = m_layout[i];
			glBufferSubData(GL_UNIFORM_BUFFER, element.offset, element.size, data[i]);
		}

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void OpenGLUniformBuffer::SetData(const std::vector<std::vector<const void*>>& dataList)
	{
		ENGINE_PROFILE_FUNCTION();

		uint32_t totalOffset = 0;
		glBindBuffer(GL_UNIFORM_BUFFER, m_rendererId);
		for (uint32_t i = 0; i < dataList.size(); ++i)
		{
			const std::vector<const void*> data = dataList[i];
			ENGINE_CORE_ASSERT(m_layout.GetNumOfElements() == data.size(), "Incorrect number of data");
			
			for (uint32_t j = 0; j < m_layout.GetNumOfElements(); ++j)
			{
				const BufferElement& element = m_layout[j];
				glBufferSubData(GL_UNIFORM_BUFFER, totalOffset + element.offset, element.size, data[j]);
			}

			totalOffset += m_layout.GetStride();
			ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
		}
	}
}
