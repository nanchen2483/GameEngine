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

		inline virtual std::string GetFilePath(uint32_t index) const override { return m_faces[index]; }
		inline virtual uint32_t GetWidth(uint32_t index) const override { return m_width[index]; }
		inline virtual uint32_t GetHeight(uint32_t index) const override { return m_height[index]; }
		inline virtual uint32_t GetRendererId() const override { return m_rendererId; }
		inline virtual TextureType GetType() const override { return m_type; }
		
		virtual void SetData(void* data, uint32_t size) override { ENGINE_CORE_ERROR("Not implemented"); };
		virtual void BindImage(uint32_t slot, TextureAccessType access) const override;
		virtual void Bind(uint32_t slot) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_rendererId == ((OpenGLTexture3D&)other).m_rendererId;
		}
	private:
		uint32_t m_rendererId;
		const TextureType m_type = TextureType::Skybox;
		std::filesystem::path m_filePath;
		std::filesystem::path m_directory = m_filePath.parent_path();
		std::vector<std::string> m_faces;
		std::vector<uint32_t> m_width, m_height;
		TextureFormat m_format;
	};
}
