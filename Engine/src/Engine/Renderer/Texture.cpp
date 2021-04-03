#include "enginepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine
{
    Ptr<Texture2D> Texture2D::Create(const std::string& path)
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return Ptr<Texture2D>(new OpenGLTexture2D(path));
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
    }
}
