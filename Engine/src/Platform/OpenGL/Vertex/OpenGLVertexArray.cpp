#include "enginepch.h"
#include "OpenGLVertexArray.h"
#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <glad/glad.h>

namespace Engine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:		return GL_FLOAT;
		case Engine::ShaderDataType::Float2:	return GL_FLOAT;
		case Engine::ShaderDataType::Float3:	return GL_FLOAT;
		case Engine::ShaderDataType::Float4:	return GL_FLOAT;
		case Engine::ShaderDataType::Mat3:		return GL_FLOAT;
		case Engine::ShaderDataType::Mat4:		return GL_FLOAT;
		case Engine::ShaderDataType::Int:		return GL_INT;
		case Engine::ShaderDataType::Int2:		return GL_INT;
		case Engine::ShaderDataType::Int3:		return GL_INT;
		case Engine::ShaderDataType::Int4:		return GL_INT;
		case Engine::ShaderDataType::Bool:		return GL_BOOL;
		}

		ENGINE_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererId);
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_rendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer)
	{
		ENGINE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size() > 0, "Vertex Buffer has no layout!");

		glBindVertexArray(m_rendererId);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			switch (element.type)
			{
			case Engine::ShaderDataType::Float:
			case Engine::ShaderDataType::Float2:
			case Engine::ShaderDataType::Float3:
			case Engine::ShaderDataType::Float4:
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(
					index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);
				index++;
				break;
			case Engine::ShaderDataType::Int:
			case Engine::ShaderDataType::Int2:
			case Engine::ShaderDataType::Int3:
			case Engine::ShaderDataType::Int4:
			case Engine::ShaderDataType::Bool:
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(
					index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					layout.GetStride(),
					(const void*)element.offset);
				index++;
				break;
			case Engine::ShaderDataType::Mat3:
			case Engine::ShaderDataType::Mat4:
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
		}

		m_vertexBuffers.push_back(vertexBuffer);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ptr<IndexBuffer>& indexBuffer)
	{
		ENGINE_CORE_ASSERT(m_vertexBuffers.size() > 0, "Vertex Buffer is empty!");

		glBindVertexArray(m_rendererId);
		indexBuffer->Bind();

		m_indexBuffer = indexBuffer;
	}

	const uint32_t OpenGLVertexArray::GetNumOfIndices() const
	{
		if (m_indexBuffer == nullptr)
		{
			return 0;
		}

		return m_indexBuffer->GetNumOfIndices();
	}
}
