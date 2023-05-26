#pragma once
#include "Engine/Renderer/Buffer/IUniformBuffer.h"
#include "Engine/Renderer/Buffer/BufferLayout.h"

namespace Engine
{
	class OpenGLUniformBuffer : public IUniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t index, const BufferLayout layout, uint32_t numOfUniforms);
		~OpenGLUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(const std::vector<const void*>& data) override;
		virtual void SetData(const std::vector<std::vector<const void*>>& data) override;
	private:
		uint32_t m_rendererId;
		BufferLayout m_layout;
	};
}
