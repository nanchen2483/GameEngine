#pragma once
#include "Texture.h"

#include <unordered_map>

namespace Engine
{
	class TextureLibrary
	{
	public:
		void Add(Ptr<Texture2D> texture);
		void Add(Ptr<Image> image);

		Ptr<Texture2D> Load(const std::string& filePath, const TextureType type, bool flipVertically);
		Ptr<Texture2D> Load(Ptr<Image> image, const TextureType type);

		Ptr<Texture2D> Get(const std::string& filePath);
		bool Exists(const std::string& filePath);

		static TextureLibrary* GetInstance();
	private:
		TextureLibrary() = default;

		std::unordered_map<std::string, Ptr<Texture2D>> m_textures;
		static TextureLibrary* s_instance;
	};
}
