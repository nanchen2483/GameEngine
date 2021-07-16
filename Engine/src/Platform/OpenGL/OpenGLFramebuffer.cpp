#include "enginepch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Engine
{
	static const uint32_t MAX_FAMEBUFFER_SIZE = 8192;

	namespace Utils {

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outId, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outId);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			GLenum textureTarget = TextureTarget(multisampled);
			if (multisampled)
			{
				glTexImage2DMultisample(textureTarget, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(textureTarget, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget, id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;
			GLenum textureTarget = TextureTarget(multisampled);
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

		}

		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
			case Engine::FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}

			return false;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_specification(spec)
	{
		for (auto format : m_specification.attachments.attachments)
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
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_rendererId);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
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

		bool multisample = m_specification.samples > 1;

		// Attachments
		if (m_colorAttachmentSpecifications.size())
		{
			m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());
			for (int i = 0; i < m_colorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_colorAttachments[i]);

				switch (m_colorAttachmentSpecifications[i].textureFormat)
				{
				case FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(
						m_colorAttachments[i],
						m_specification.samples,
						GL_RGBA8,
						m_specification.width,
						m_specification.height,
						i);
					break;
				}
			}
		}

		if (m_depthAttachmentSpecification.textureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_depthAttachment, 1);
			Utils::BindTexture(multisample, m_depthAttachment);
			switch (m_depthAttachmentSpecification.textureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(
					m_depthAttachment,
					m_specification.samples,
					GL_DEPTH24_STENCIL8,
					GL_DEPTH_STENCIL_ATTACHMENT,
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

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > MAX_FAMEBUFFER_SIZE || height > MAX_FAMEBUFFER_SIZE)
		{
			ENGINE_CORE_WARN("Attemped to resize frambuffer to {0}x{1}", width, height);
			return;
		}
		m_specification.width = width;
		m_specification.height = height;

		Invalidate();
	}
}