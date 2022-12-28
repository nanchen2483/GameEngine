#include "enginepch.h"
#include "Image.h"

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Renderer.h"

#include <filesystem>
#include <stb_image.h>

namespace Engine
{
	Image::Image(const std::string& filePath)
		: Image(filePath, false)
	{
	}

	Image::Image(const std::string& filePath, bool flipVertically)
		: m_filePath(filePath)
	{
		m_imageName = std::filesystem::path(filePath).stem().string();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:
			// The (0, 0) coordinate of the image in OpenGL is the bottom-left corner,
			// but images usually have (0, 0) at the top-left corner.
			stbi_set_flip_vertically_on_load(!flipVertically);
			break;
		case RendererAPI::API::None:
		default:
			ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported");
			break;
		}
		
		m_data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 0);

		ENGINE_CORE_ASSERT(m_data, "Failed to load image: " + filePath + "!");
	}
	
	Image::~Image()
	{
		stbi_image_free(m_data);
	}
}
