#include "enginepch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
        : m_filePath(filePath), m_width(0), m_height(0), m_rendererId(0)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        ENGINE_CORE_ASSERT(data, "Failed to load image!");

        m_width = width;
        m_height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else if (channels == 4)
        {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
        }

        ENGINE_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
        glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_rendererId);
    }

    void Engine::OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_rendererId);
    }
}
