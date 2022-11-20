#include "enginepch.h"
#include "Image.h"

#include "Engine/Core/Base.h"

#include <filesystem>
#include <stb_image.h>

namespace Engine
{
	Image::Image(const std::string& filePath, const bool flipVertically)
		: m_filePath(filePath)
	{
		m_imageName = std::filesystem::path(filePath).stem().string();

		stbi_set_flip_vertically_on_load(flipVertically);
		m_data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channels, 0);

		ENGINE_CORE_ASSERT(m_data, "Failed to load image: " + filePath + "!");
	}
	
	Image::~Image()
	{
		stbi_image_free(m_data);
	}
}
