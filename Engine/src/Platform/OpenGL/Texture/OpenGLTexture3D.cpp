#include "enginepch.h"
#include "OpenGLTexture3D.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"
#include "Platform/Util/OpenGLUtil.h"

#include <numeric>
#include <stb_image.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLTexture3D::OpenGLTexture3D(const std::vector<Ptr<Image>>& faces)
	{
		ENGINE_PROFILE_FUNCTION();
		Init(faces);
		m_uid = Uid::NewUid(std::accumulate(m_faces.begin(), m_faces.end(), std::string{}));

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, m_formats.front().internalFormat, faces.front()->GetWidth(), faces.front()->GetHeight());
		
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		for (uint32_t i = 0; i < faces.size(); i++)
		{
			glTextureSubImage3D(m_rendererId, 0, 0, 0, i, m_widths[i], m_heights[i], 1, m_formats[i].dataFormat, GL_UNSIGNED_BYTE, faces[i]->GetData());
		}

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void OpenGLTexture3D::Init(const std::vector<Ptr<Image>>& faces)
	{
		ENGINE_CORE_ASSERT(faces.size() == 6, "3D textures must have 6 faces");

		m_textureName = std::filesystem::path(faces.front()->GetFilePath()).parent_path().filename().string();
		m_faces.reserve(faces.size());
		m_widths.reserve(faces.size());
		m_heights.reserve(faces.size());
		m_formats.reserve(faces.size());

		for (uint32_t i = 0; i < faces.size(); i++)
		{
			Ptr<Image> face = faces[i];
			m_faces.push_back(face->GetFilePath());
			m_widths.push_back(face->GetWidth());
			m_heights.push_back(face->GetHeight());

			TextureFormat format{};
			switch (face->GetChannels())
			{
			case 1:
				format.internalFormat = GL_RED;
				format.dataFormat = GL_RED;
				format.channels = 1;
				break;
			case 3:
				format.internalFormat = GL_RGB8;
				format.dataFormat = GL_RGB;
				format.channels = 3;
				break;
			case 4:
				format.internalFormat = GL_RGBA8;
				format.dataFormat = GL_RGBA;
				format.channels = 4;
				break;
			default:
				format.internalFormat = GL_NONE;
				format.dataFormat = GL_NONE;
				format.channels = 0;
				ENGINE_CORE_ERROR("Unsupported channel");
				break;
			}

			ENGINE_CORE_ASSERT(format.internalFormat & format.dataFormat, "Format not supported!");
			m_formats.push_back(format);
		}
	}

	OpenGLTexture3D::~OpenGLTexture3D()
	{
		glDeleteTextures(1, &m_rendererId);
	}

	void OpenGLTexture3D::SetData(void* data, TextureOrientationType type, uint32_t size)
	{
		uint32_t index = (uint32_t)type;
		uint32_t bpp = m_formats[index].channels;
		ENGINE_CORE_ASSERT(size == m_widths[index] * m_heights[index] * bpp, "Data must be entire texture!");
		glTextureSubImage3D(m_rendererId, 0, 0, 0, index, m_widths[index], m_heights[index], 1, m_formats[index].dataFormat, GL_UNSIGNED_BYTE, data);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void OpenGLTexture3D::BindImage(uint32_t slot, TextureAccessType access) const
	{
		glBindImageTexture(slot, m_rendererId, 0, false, 0, OpenGLUtil::ToGL(access), m_formats.front().internalFormat);
	}

	void OpenGLTexture3D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_rendererId);
	}
}
