#include "TextureLibrary.h"

#include <filesystem>

namespace Engine
{
	TextureLibrary* TextureLibrary::s_instance = nullptr;

	void TextureLibrary::Add(Ptr<Texture2D> texture)
	{
		std::string path = texture->GetFilePath();
		ENGINE_CORE_ASSERT(!Exists(path), "Texture already exists!");
		m_textures[path] = texture;
	}

	void TextureLibrary::Add(Ptr<Image> image)
	{
		std::string path = image->GetFilePath();
		ENGINE_CORE_ASSERT(!Exists(path), "Texture already exists!");
		Ptr<Texture2D> texture = Texture2D::Create(image);
		m_textures[path] = texture;
	}

	Ptr<Texture2D> TextureLibrary::Load(const std::string& filePath, const TextureType type, bool flipVertically)
	{
		if (Exists(filePath))
		{
			return Get(filePath);
		}

		Ptr<Texture2D> texture = Texture2D::Create(filePath, type, flipVertically);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Load(Ptr<Image> image, const TextureType type)
	{
		if (Exists(image->GetFilePath()))
		{
			return Get(image->GetFilePath());
		}

		Ptr<Texture2D> texture = Texture2D::Create(image, type);
		Add(texture);

		return texture;
	}

	Ptr<Texture2D> TextureLibrary::Get(const std::string& filePath)
	{
		ENGINE_CORE_ASSERT(Exists(filePath), "Texture not found!");
		return m_textures[filePath];
	}
	
	bool TextureLibrary::Exists(const std::string& filePath)
	{
		return m_textures.find(filePath) != m_textures.end();
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
