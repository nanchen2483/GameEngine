#include "enginepch.h"
#include "OpenGLTexture3D.h"

#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLTexture3D::OpenGLTexture3D(const std::vector<Ptr<Image>>& faces)
	{
		ENGINE_PROFILE_FUNCTION();
		ENGINE_CORE_ASSERT(faces.size() == 6, "3D textures must have 6 faces");

		m_faces.reserve(faces.size());
		m_width.reserve(faces.size());
		m_height.reserve(faces.size());
		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		for (uint32_t i = 0; i < faces.size(); i++)
		{
			Ptr<Image> face = faces[i];
			m_faces.push_back(face->GetFilePath());
			m_width.push_back(face->GetWidth());
			m_height.push_back(face->GetHeight());

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, face->GetInternalFormat(), m_width[i], m_height[i], 0, face->GetDataFormat(), GL_UNSIGNED_BYTE, face->GetData());
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}


	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &m_rendererId);
	}

	void OpenGLTexture3D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererId);
	}
}
