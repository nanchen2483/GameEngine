#include "enginepch.h"
#include "OpenGLTexture2D.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height)
	{
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const Ptr<Image> image, const TextureType type)
		: m_filePath(image->GetFilePath()), m_type(type), m_width(image->GetWidth()), m_height(image->GetHeight())
	{
		ENGINE_PROFILE_FUNCTION();

		GLenum internalFormat = 0, dataFormat = 0;
		if (image->GetChannels() == 1)
		{
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}
		else if (image->GetChannels() == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (image->GetChannels() == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		ENGINE_CORE_ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, image->GetData());
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, const TextureType type, const bool flipVertically)
		: m_filePath(filePath), m_type(type), m_width(0), m_height(0)
	{
		ENGINE_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(flipVertically);
		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		ENGINE_CORE_ASSERT(data, "Failed to load image!");

		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 1)
		{
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		ENGINE_CORE_ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererId);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
		ENGINE_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	void Engine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererId);
	}
}
