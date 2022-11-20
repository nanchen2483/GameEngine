#pragma once
#include "Image.h"
#include "Enum/TextureType.h"

#include <map>
#include <string>
#include <vector>

namespace Engine
{
	struct TextureFormat
	{
		uint32_t internalFormat;
		uint32_t dataFormat;
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetRendererId() const = 0;
		virtual TextureType GetType() const = 0;
		virtual const std::string& GetName() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void BindImage(uint32_t slot = 0, TextureAccessType access = TextureAccessType::ReadOnly) const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual std::string GetFilePath() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::vector<float>& GetData() = 0;

		static Ptr<Texture2D> Create(uint32_t width, uint32_t height, uint32_t levels = 1, TextureFormatType format = TextureFormatType::RGBA8);
		static Ptr<Texture2D> Create(const std::string& filePath, const TextureType type = TextureType::None, bool flipVertically = true);
		static Ptr<Texture2D> Create(const Ptr<Image> image, const TextureType type = TextureType::None);
	};

	class Texture3D : public Texture
	{
	public:
		virtual std::string GetFilePath(uint32_t index) const = 0;
		virtual uint32_t GetWidth(uint32_t index) const = 0;
		virtual uint32_t GetHeight(uint32_t index) const = 0;

		static Ptr<Texture3D> Create(const std::vector<Ptr<Image>>& faces);
	};

	typedef std::map<std::string, Ptr<Texture2D>> TextureMap;
}
