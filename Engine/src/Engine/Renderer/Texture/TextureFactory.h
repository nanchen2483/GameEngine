#pragma once
#include "ITexture.h"

namespace Engine
{
	class ENGINE_API TextureFactory
	{
	public:
		static Ptr<ITexture3D> Create3D(const std::vector<Ptr<Image>>& faces);
	private:
		static Ptr<ITexture2D> Create2D(uint32_t width, uint32_t height, uint32_t levels = 1, TextureFormatType format = TextureFormatType::RGBA8);
		static Ptr<ITexture2D> Create2D(const std::string& filePath, const TextureType type = TextureType::None, bool flipVertically = false);
		static Ptr<ITexture2D> Create2D(const Ptr<Image> image, const TextureType type = TextureType::None);

		friend class TextureLibrary;
	};
}
