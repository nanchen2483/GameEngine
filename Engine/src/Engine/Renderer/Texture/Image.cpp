#include "enginepch.h"
#include "Image.h"

#include <stb_image.h>
#include <glad/glad.h>

#include "Engine/Core/Base.h"

namespace Engine
{
	Image::Image(const std::string& filePath, const bool flipVertically)
		: m_filePath(filePath)
	{
		int32_t channels;
		stbi_set_flip_vertically_on_load(flipVertically);
		m_data = stbi_load(filePath.c_str(), &m_width, &m_height, &channels, 0);
		ENGINE_CORE_ASSERT(m_data, "Failed to load image!");

		switch (channels)
		{
		case 1:
			m_internalFormat = GL_RED;
			m_dataFormat = GL_RED;
			break;
		case 3:
			m_internalFormat = GL_RGB8;
			m_dataFormat = GL_RGB;
			break;
		case 4:
			m_internalFormat = GL_RGBA8;
			m_dataFormat = GL_RGBA;
			break;
		default:
			m_internalFormat = GL_NONE;
			m_dataFormat = GL_NONE;
			ENGINE_CORE_ERROR("Unsupported channel");
			break;
		}

		ENGINE_CORE_ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");
	}
	
	Image::~Image()
	{
		stbi_image_free(m_data);
	}
}
