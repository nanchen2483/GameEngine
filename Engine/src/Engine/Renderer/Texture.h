#pragma once

#include <string>

namespace Engine
{
	enum class TextureType
	{
		None = 0,
		Diffuse,
		Specular,
		Normal,
		Height
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererId() const = 0;
		virtual std::string GetFilePath() const = 0;
		virtual TextureType GetType() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ptr<Texture2D> Create(uint32_t width, uint32_t height);
		static Ptr<Texture2D> Create(const std::string& filePath, const TextureType type = TextureType::None, bool flipVertically = true);
	};
}
