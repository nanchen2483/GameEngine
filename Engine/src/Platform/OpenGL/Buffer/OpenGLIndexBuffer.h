#pragma once

#include "Engine/Renderer/Buffer/Buffer.h"

namespace Engine
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t numOfIndices);
		~OpenGLIndexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetNumOfIndices() const { return m_numOfIndices; }
	private:
		uint32_t m_rendererId;
		const uint32_t m_numOfIndices;
	};
}
