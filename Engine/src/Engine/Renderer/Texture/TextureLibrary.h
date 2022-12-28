#pragma once
#include "Engine/Library/ILibrary.h"
#include "Texture.h"

namespace Engine
{
	class TextureLibrary : public ILibrary<Texture2D>
	{
	public:
		virtual Ptr<Texture2D> Load(const std::filesystem::path& filePath) override;
		Ptr<Texture2D> Load(const std::filesystem::path&, const TextureType type, bool flipVertically = false);
		Ptr<Texture2D> Load(const Ptr<Image> image, const TextureType type);
		Ptr<Texture2D> Load(uint32_t height, uint32_t width, uint32_t levels = 1, TextureFormatType format = TextureFormatType::RGBA8);

		static TextureLibrary* GetInstance();
	private:
		TextureLibrary() = default;
		virtual Ptr<Texture2D> Get(const Uid& key) const override;
		virtual void Add(const Ptr<Texture2D> data) override;
		virtual bool Exists(const Uid& key) const override;

		std::unordered_map<uint64_t, Ptr<Texture2D>> m_textures;
		static TextureLibrary* s_instance;
	};
}
