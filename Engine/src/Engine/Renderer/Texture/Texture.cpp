#include "enginepch.h"
#include "Texture.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.h"
#include "Platform/OpenGL/Texture/OpenGLTexture3D.h"

namespace Engine
{
	Ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t levels, TextureFormatType format)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLTexture2D>(width, height, levels, format);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<Texture2D> Texture2D::Create(const Ptr<Image> image, const TextureType type)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLTexture2D>(image, type);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<Texture2D> Texture2D::Create(const std::string& filePath, const TextureType type, bool flipVertically)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLTexture2D>(filePath, type, flipVertically);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<Texture3D> Texture3D::Create(const std::vector<Ptr<Image>>& faces)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLTexture3D>(faces);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
