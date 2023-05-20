#pragma once
#include "Image/Image.h"
#include "Engine/Core/System/Object/IUniqueObject.h"
#include "TextureFormat.h"
#include "TextureType.h"

namespace Engine
{
	class ENGINE_API ITexture : public IUniqueObject
	{
	public:
		virtual ~ITexture() = default;
		virtual uint32_t GetRendererId() const = 0;
		virtual TextureType GetType() const = 0;
		virtual const std::string& GetName() const = 0;
		virtual void BindImage(uint32_t slot = 0, TextureAccessType access = TextureAccessType::ReadOnly) const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const ITexture& other) const = 0;
	};

	class ENGINE_API ITexture2D : public ITexture
	{
	public:
		virtual std::string GetFilePath() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const std::vector<float>& GetData() = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
	};

	class ENGINE_API ITexture3D : public ITexture
	{
	public:
		virtual std::string GetFilePath(TextureOrientationType type) const = 0;
		virtual uint32_t GetWidth(TextureOrientationType type) const = 0;
		virtual uint32_t GetHeight(TextureOrientationType type) const = 0;
		virtual void SetData(void* data, TextureOrientationType type, uint32_t size) = 0;
	};
}
