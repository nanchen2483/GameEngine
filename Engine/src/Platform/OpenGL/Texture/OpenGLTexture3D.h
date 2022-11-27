#pragma once
#include "Engine/Renderer/Texture/Texture.h"

#include <filesystem>

namespace Engine
{
	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(const std::vector<Ptr<Image>>& faces);
		~OpenGLTexture3D();

		inline virtual std::string GetFilePath(TextureOrientationType type) const override { return m_faces[(uint32_t)type]; }
		inline virtual uint32_t GetWidth(TextureOrientationType type) const override { return m_widths[(uint32_t)type]; }
		inline virtual uint32_t GetHeight(TextureOrientationType type) const override { return m_heights[(uint32_t)type]; }
		inline virtual uint32_t GetRendererId() const override { return m_rendererId; }
		inline virtual TextureType GetType() const override { return m_type; }
		inline virtual const std::string& GetName() const { return m_textureName; }

		virtual void SetData(void* data, TextureOrientationType type, uint32_t size) override;
		virtual void BindImage(uint32_t slot, TextureAccessType access) const override;
		virtual void Bind(uint32_t slot) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_rendererId == ((OpenGLTexture3D&)other).m_rendererId;
		}
	private:
		void Init(const std::vector<Ptr<Image>>& faces);

		uint32_t m_rendererId;
		const TextureType m_type = TextureType::Skybox;
		std::vector<std::string> m_faces;
		std::vector<uint32_t> m_widths, m_heights;
		std::vector<TextureFormat> m_formats;
		std::string m_textureName;
	};
}
