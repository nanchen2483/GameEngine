#include "enginepch.h"
#include "OpenGLFramebuffer.h"

#include "Platform/OpenGL/Debug/OpenGLDebug.h"

#include <glad/glad.h>

namespace Engine
{
	static const uint32_t MAX_FAMEBUFFER_SIZE = 8192;

	namespace Utils {

		static GLenum TextureTarget(bool multisampled, bool isTextureArray)
		{
			if (multisampled)
			{
				return isTextureArray ? GL_TEXTURE_2D_MULTISAMPLE_ARRAY : GL_TEXTURE_2D_MULTISAMPLE;
			}
			else
			{
				return isTextureArray ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
			}
		}

		static void CreateTextures(bool multisampled, bool isTextureArray, uint32_t* outId, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled, isTextureArray), count, outId);
		}

		static void BindTexture(bool multisampled, bool isTextureArray, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled, isTextureArray), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, int arraysize, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			bool isTextureArray = arraysize > 1;
			GLenum textureTarget = TextureTarget(multisampled, isTextureArray);
			if (multisampled)
			{
				glTexImage2DMultisample(textureTarget, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(textureTarget, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget, id, 0);

			ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
		}

		static void AttachDepthTexture(uint32_t id, int samples, int arraysize, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			bool isTextureArray = arraysize > 1;
			GLenum textureTarget = TextureTarget(multisampled, isTextureArray);
			if (multisampled)
			{
				glTexImage2DMultisample(textureTarget, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(textureTarget, 1, format, width, height);

				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget, id, 0);

			ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
		}

		static void AttachDepthTextureArray(uint32_t id, int samples, int arraysize, GLenum internalFormat, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			bool isTextureArray = arraysize > 1;
			GLenum textureTarget = TextureTarget(multisampled, isTextureArray);
			if (multisampled)
			{
				glTexImage3DMultisample(textureTarget, samples, internalFormat, width, height, arraysize, GL_FALSE);
			}
			else
			{
				glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, width, height, arraysize, 0, format, GL_FLOAT, nullptr);

				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture(GL_FRAMEBUFFER, attachmentType, id, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
			case FramebufferTextureFormat::DEPTH_ARRAY:
				return true;
			}

			return false;
		}

		static GLenum GetGLTextureFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case Engine::FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case Engine::FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			case Engine::FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
			}
			
			ENGINE_CORE_ASSERT(false, "Invalid framebuffer texture format");
			return -1;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_specification(spec)
	{
		for (FramebufferTextureSpecification format : m_specification.attachments.attachments)
		{
			if (!Utils::IsDepthFormat(format.textureFormat))
			{
				m_colorAttachmentSpecifications.emplace_back(format);
			}
			else
			{
				m_depthAttachmentSpecification = format;
			}
		}

		Setup();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererId);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Setup()
	{
		if (m_rendererId)
		{
			glDeleteFramebuffers(1, &m_rendererId);
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			glDeleteTextures(1, &m_depthAttachment);

			m_colorAttachments.clear();
			m_depthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_rendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

		bool multisampled = m_specification.samples > 1;
		bool isTextureArray = m_specification.arraySize > 1;

		// Attachments
		if (m_colorAttachmentSpecifications.size())
		{
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Utils::CreateTextures(multisampled, isTextureArray, m_colorAttachments.data(), m_colorAttachments.size());
			for (uint32_t i = 0; i < m_colorAttachments.size(); i++)
			{
				Utils::BindTexture(multisampled, isTextureArray, m_colorAttachments[i]);

				switch (m_colorAttachmentSpecifications[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(
						m_colorAttachments[i],
						m_specification.samples,
						m_specification.arraySize,
						GL_RGBA8,
						GL_RGBA,
						m_specification.width,
						m_specification.height,
						i);
					break;
				case FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(
						m_colorAttachments[i],
						m_specification.samples,
						m_specification.arraySize,
						GL_R32I,
						GL_RGB_INTEGER,
						m_specification.width,
						m_specification.height,
						i);
					break;
				}
			}
		}

		if (m_depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisampled, isTextureArray, &m_depthAttachment, 1);
			Utils::BindTexture(multisampled, isTextureArray, m_depthAttachment);
			switch (m_depthAttachmentSpecification.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(
					m_depthAttachment,
					m_specification.samples,
					m_specification.arraySize,
					GL_DEPTH24_STENCIL8,
					GL_DEPTH_STENCIL_ATTACHMENT,
					m_specification.width,
					m_specification.height);
				break;
			case FramebufferTextureFormat::DEPTH_ARRAY:
				Utils::AttachDepthTextureArray(
					m_depthAttachment,
					m_specification.samples,
					m_specification.arraySize,
					GL_DEPTH_COMPONENT32F,
					GL_DEPTH_COMPONENT,
					GL_DEPTH_ATTACHMENT,
					m_specification.width,
					m_specification.height);
				break;
			}
		}

		if (m_colorAttachments.size() > 1)
		{
			ENGINE_CORE_ASSERT(m_colorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else if (m_colorAttachments.empty())
		{
			// Only depth
			glDrawBuffer(GL_NONE);
		}

		ENGINE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ENGINE_CORE_ASSERT(OpenGLDebug::IsValid(), OpenGLDebug::GetErrorMessage());
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindDepthTexture(uint32_t slot)
	{
		glBindTextureUnit(slot, m_depthAttachment);
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		ENGINE_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "Invalid attachment index {0}", attachmentIndex);

		FramebufferTextureSpecification& spec = m_colorAttachmentSpecifications[attachmentIndex];

		glClearTexImage(m_colorAttachments[attachmentIndex], 0, Utils::GetGLTextureFormat(spec.textureFormat), GL_INT, &value);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MAX_FAMEBUFFER_SIZE || height > MAX_FAMEBUFFER_SIZE)
		{
			ENGINE_CORE_WARN("Attemped to resize frambuffer to {0}x{1}", width, height);
			return;
		}
		m_specification.width = width;
		m_specification.height = height;

		Setup();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		ENGINE_CORE_ASSERT(attachmentIndex < m_colorAttachments.size(), "Invalid attachment index {0}", attachmentIndex);

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixel;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
		return pixel;
	}
}
