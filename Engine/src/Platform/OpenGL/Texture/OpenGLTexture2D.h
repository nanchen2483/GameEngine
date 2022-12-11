#pragma once
#include "Engine/Renderer/Texture/Texture.h"

namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t levels, TextureFormatType format);
		OpenGLTexture2D(const std::string& filePath, const TextureType type, bool flipVertically = true);
		OpenGLTexture2D(const Ptr<Image> image, const TextureType type);
		~OpenGLTexture2D();

		inline virtual std::string GetFilePath() const override { return m_filePath; }
		inline virtual uint32_t GetWidth() const override { return m_width; };
		inline virtual uint32_t GetHeight() const override { return m_height; };
		inline virtual uint32_t GetRendererId() const override { return m_rendererId; }
		inline virtual TextureType GetType() const override { return m_type; }
		inline virtual const std::string& GetName() const { return m_textureName; }

		virtual const std::vector<float>& GetData() override;
		virtual void SetData(void* data, uint32_t size) override;
		virtual void BindImage(uint32_t slot, TextureAccessType access) const override;
		virtual void Bind(uint32_t slot) const override;
		
		virtual bool operator==(const Texture& other) const override
		{
			return m_rendererId == ((OpenGLTexture2D&)other).m_rendererId;
		}
	private:
		uint32_t m_rendererId;
		std::string m_filePath;
		uint32_t m_width, m_height;
		TextureType m_type;
		TextureFormat m_format;
		std::string m_textureName;

		std::vector<float> m_pixels;
	};
}
