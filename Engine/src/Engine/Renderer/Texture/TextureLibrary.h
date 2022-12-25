#pragma once
#include "Engine/Library/ILibrary.h"
#include "Texture.h"

namespace Engine
{
	class TextureLibrary : public ILibrary<Texture2D>
	{
	public:
		virtual Ptr<Texture2D> Load(const std::string filePath) override;
		virtual Ptr<Texture2D> Get(const std::string& filePath) const override;
		virtual void Add(const Ptr<Texture2D> data) override;

		Ptr<Texture2D> Load(const std::string& filePath, const TextureType type, bool flipVertically = false);
		Ptr<Texture2D> Load(const Ptr<Image> image, const TextureType type);
		void Add(const Ptr<Image> image);

		static TextureLibrary* GetInstance();
	protected:
		virtual bool Exists(const std::string& filePath) const override;
	private:
		TextureLibrary() = default;

		std::unordered_map<std::string, Ptr<Texture2D>> m_textures;
		static TextureLibrary* s_instance;
	};
}
