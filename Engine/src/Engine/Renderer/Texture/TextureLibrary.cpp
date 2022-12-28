#include "enginepch.h"
#include "TextureLibrary.h"

#include <filesystem>

namespace Engine
{
	TextureLibrary* TextureLibrary::s_instance = nullptr;

	Ptr<Texture2D> TextureLibrary::Load(const std::filesystem::path& filePath)
	{
		return Load(filePath, TextureType::Diffuse, false);
	}

	Ptr<Texture2D> TextureLibrary::Load(const std::filesystem::path& filePath, const TextureType type, bool flipVertically)
	{
		const Uid uid = Uid::NewUid(filePath.string());
		if (Exists(uid))
		{
			return Get(uid);
		}

		Ptr<Texture2D> texture = Texture2D::Create(filePath.string(), type, flipVertically);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Load(Ptr<Image> image, const TextureType type)
	{
		const Uid uid = Uid::NewUid(image->GetFilePath());
		if (Exists(uid))
		{
			return Get(uid);
		}

		Ptr<Texture2D> texture = Texture2D::Create(image, type);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Load(uint32_t height, uint32_t width, uint32_t levels, TextureFormatType format)
	{
		const std::string name = "Texture_w:" + std::to_string(width) + "_h:" + std::to_string(height) + "_f:" + std::to_string((int)format);
		const Uid uid = Uid::NewUid(name);
		if (Exists(uid))
		{
			return Get(uid);
		}

		Ptr<Texture2D> texture = Texture2D::Create(height, width, levels, format);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Get(const Uid& key) const
	{
		ENGINE_CORE_ASSERT(Exists(key), "Texture not found!");
		return m_textures.at(key);
	}

	void TextureLibrary::Add(const Ptr<Texture2D> data)
	{
		const Uid& uid = data->GetUid();
		ENGINE_CORE_ASSERT(!Exists(uid), "Texture already exists!");
		m_textures.insert({ uid, data });
	}
	
	bool TextureLibrary::Exists(const Uid& key) const
	{
		return m_textures.find(key) != m_textures.end();
	}

	TextureLibrary* TextureLibrary::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new TextureLibrary();
		}

		return s_instance;
	}
}
