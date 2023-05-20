#pragma once
#include "Engine/Renderer/Buffer/UniformBuffer/IUniformBuffer.h"
#include "Engine/Renderer/Buffer/BufferLayout.h"

namespace Engine
{
	class OpenGLUniformBuffer : public IUniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t index, const BufferLayout layout);
		~OpenGLUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(const std::vector<const void*>& data) override;
	private:
		uint32_t m_rendererId;
		BufferLayout m_layout;
	};
}