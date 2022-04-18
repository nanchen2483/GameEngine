#include "enginepch.h"
#include "Texture.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/Texture/OpenGLTexture2D.h"

namespace Engine
{
	Ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreatePtr<OpenGLTexture2D>(width, height);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	Ptr<Texture2D> Texture2D::Create(const Ptr<Image> image, const TextureType type)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreatePtr<OpenGLTexture2D>(image, type);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	Ptr<Texture2D> Texture2D::Create(const std::string& filePath, const TextureType type, bool flipVertically)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreatePtr<OpenGLTexture2D>(filePath, type, flipVertically);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}
