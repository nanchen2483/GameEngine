#include "enginepch.h"
#include "OpenGLTexture2D.h"
#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_width(width), m_height(height), m_type(TextureType::None)
	{
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, const TextureType type, const bool flipVertically)
		: OpenGLTexture2D(CreatePtr<Image>(filePath, flipVertically), type)
	{
	}

	OpenGLTexture2D::OpenGLTexture2D(const Ptr<Image> image, const TextureType type)
		: m_filePath(image->GetFilePath()), m_type(type), m_width(image->GetWidth()), m_height(image->GetHeight())
	{
		ENGINE_PROFILE_FUNCTION();

		int32_t channels = image->GetChannels();
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

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, image->GetData());

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
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

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void Engine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererId);
	}
}
