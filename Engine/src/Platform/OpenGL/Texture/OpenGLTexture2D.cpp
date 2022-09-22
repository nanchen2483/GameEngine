#include "enginepch.h"
#include "OpenGLTexture2D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/Util/OpenGLUtil.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, uint32_t levels, TextureFormatType format)
		: m_width(width), m_height(height), m_format(OpenGLUtil::ToGL(format)), m_type(TextureType::None)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, levels, m_format.internalFormat, m_width, m_height);

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
			m_format.internalFormat = GL_RED;
			m_format.dataFormat = GL_RED;
			break;
		case 3:
			m_format.internalFormat = GL_RGB8;
			m_format.dataFormat = GL_RGB;
			break;
		case 4:
			m_format.internalFormat = GL_RGBA8;
			m_format.dataFormat = GL_RGBA;
			break;
		default:
			m_format.internalFormat = GL_NONE;
			m_format.dataFormat = GL_NONE;
			ENGINE_CORE_ERROR("Unsupported channel");
			break;
		}

		ENGINE_CORE_ASSERT(m_format.internalFormat & m_format.dataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_format.internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_format.dataFormat, GL_UNSIGNED_BYTE, image->GetData());

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_rendererId);
	}

	const std::vector<float>& OpenGLTexture2D::GetData()
	{
		const uint32_t TERRAIN_SIZE = m_width * m_height;

		float *buffer = new float[TERRAIN_SIZE];
		glGetTextureImage(m_rendererId, 0, GL_RED, GL_FLOAT, TERRAIN_SIZE * 4, buffer);
		m_pixels = std::vector<float>(buffer, buffer + TERRAIN_SIZE);
		delete[] buffer;

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());

		return m_pixels;
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_format.dataFormat == GL_RGBA ? 4 : 3;
		ENGINE_CORE_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_format.dataFormat, GL_UNSIGNED_BYTE, data);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void OpenGLTexture2D::BindImage(uint32_t slot, TextureAccessType access) const
	{
		glBindImageTexture(slot, m_rendererId, 0, false, 0, OpenGLUtil::ToGL(access), m_format.internalFormat);
	}

	void Engine::OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererId);
	}
}
