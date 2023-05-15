#include "enginepch.h"
#include "TextureLibrary.h"

namespace Engine
{
	Ptr<ITexture2D> TextureLibrary::Load(const std::filesystem::path& filePath)
	{
		return Load(filePath, TextureType::Diffuse, false);
	}

	Ptr<ITexture2D> TextureLibrary::Load(const std::filesystem::path& filePath, const TextureType type, bool flipVertically)
	{
		TextureLibrary& instance = GetInstance();

		const Uid uid = Uid::NewUid(filePath.string());
		if (instance.Exists(uid))
		{
			return instance.Get(uid);
		}

		Ptr<ITexture2D> texture = ITexture2D::Create(filePath.string(), type, flipVertically);
		instance.Add(texture);

		return texture;
	}

	Ptr<ITexture2D> TextureLibrary::Load(Ptr<Image> image, const TextureType type)
	{
		TextureLibrary& instance = GetInstance();

		const Uid uid = Uid::NewUid(image->GetFilePath());
		if (instance.Exists(uid))
		{
			return instance.Get(uid);
		}

		Ptr<ITexture2D> texture = ITexture2D::Create(image, type);
		instance.Add(texture);

		return texture;
	}

	Ptr<ITexture2D> TextureLibrary::Load(uint32_t height, uint32_t width, uint32_t levels, TextureFormatType format)
	{
		TextureLibrary& instance = GetInstance();

		const std::string name = "Texture_w:" + std::to_string(width) + "_h:" + std::to_string(height) + "_f:" + std::to_string((int)format);
		const Uid uid = Uid::NewUid(name);
		if (instance.Exists(uid))
		{
			return instance.Get(uid);
		}

		Ptr<ITexture2D> texture = ITexture2D::Create(height, width, levels, format);
		instance.Add(texture);

		return texture;
	}

	Ptr<ITexture2D> TextureLibrary::Get(const Uid& key) const
	{
		ENGINE_CORE_ASSERT(Exists(key), "Texture not found!");
		return m_textures.at(key);
	}

	void TextureLibrary::Add(const Ptr<ITexture2D> data)
	{
		const Uid& uid = data->GetUid();
		ENGINE_CORE_ASSERT(!Exists(uid), "Texture already exists!");
		m_textures.emplace(uid, data);
	}
	
	bool TextureLibrary::Exists(const Uid& key) const
	{
		return m_textures.find(key) != m_textures.end();
	}

	TextureLibrary& TextureLibrary::GetInstance()
	{
		static TextureLibrary instance;
		return instance;
	}
}
