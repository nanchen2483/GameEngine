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
		const std::string name = filePath.stem().string();
		if (Exists(name))
		{
			return Get(name);
		}

		Ptr<Texture2D> texture = Texture2D::Create(filePath.string(), type, flipVertically);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Load(Ptr<Image> image, const TextureType type)
	{
		if (Exists(image->GetName()))
		{
			return Get(image->GetName());
		}

		Ptr<Texture2D> texture = Texture2D::Create(image, type);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Load(uint32_t height, uint32_t width, uint32_t levels, TextureFormatType format)
	{
		const std::string name = "Texture_w:" + std::to_string(width) + "_h:" + std::to_string(height) + "_f:" + std::to_string((int)format);
		if (Exists(name))
		{
			return Get(name);
		}

		Ptr<Texture2D> texture = Texture2D::Create(height, width, levels, format);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Get(const std::string& key) const
	{
		ENGINE_CORE_ASSERT(Exists(key), "Texture not found!");
		return m_textures.at(key);
	}

	void TextureLibrary::Add(const Ptr<Texture2D> data)
	{
		std::string path = data->GetName();
		ENGINE_CORE_ASSERT(!Exists(path), "Texture already exists!");
		m_textures.insert({ path, data });
	}
	
	bool TextureLibrary::Exists(const std::string& key) const
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
