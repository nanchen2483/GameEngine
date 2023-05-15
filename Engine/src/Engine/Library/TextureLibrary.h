#pragma once
#include "ILibrary.h"
#include "Engine/Renderer/Texture/ITexture.h"

namespace Engine
{
	class ENGINE_API TextureLibrary : public ILibrary<ITexture2D>
	{
	public:
		static Ptr<ITexture2D> Load(const std::filesystem::path& filePath);
		static Ptr<ITexture2D> Load(const std::filesystem::path&, const TextureType type, bool flipVertically = false);
		static Ptr<ITexture2D> Load(const Ptr<Image> image, const TextureType type);
		static Ptr<ITexture2D> Load(uint32_t height, uint32_t width, uint32_t levels = 1, TextureFormatType format = TextureFormatType::RGBA8);
	private:
		TextureLibrary() = default;
		virtual Ptr<ITexture2D> Get(const Uid& key) const override;
		virtual void Add(const Ptr<ITexture2D> data) override;
		virtual bool Exists(const Uid& key) const override;
		static TextureLibrary& GetInstance();

		std::unordered_map<uint64_t, Ptr<ITexture2D>> m_textures;
	};
}
