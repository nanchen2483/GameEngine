#pragma once

#include "Engine/Renderer/VertexArray.h"
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

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ptr<VertexBuffer>>& GetVertexBuffer() const { return m_vertexBuffers; };
		virtual const Ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; };
	private:
		uint32_t m_rendererId;
		std::vector<Ptr<VertexBuffer>> m_vertexBuffers;
		Ptr<IndexBuffer> m_indexBuffer;
	};
}