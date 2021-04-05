#pragma once

#include "Engine/Renderer/Texture.h"

namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& filePath);
		~OpenGLTexture2D();

		virtual void SetData(void* data, uint32_t size) override;

		inline virtual uint32_t GetWidth() const override { return m_width; };
		inline virtual uint32_t GetHeight() const override { return m_height; };
		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_filePath;
		uint32_t m_width, m_height;
		uint32_t m_rendererId;
		unsigned int m_internalFormat;
		unsigned int m_dataFormat;
	};
}