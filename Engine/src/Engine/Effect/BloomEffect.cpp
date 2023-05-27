#include "BloomEffect.h"

#include "Engine/Factory/BufferFactory.h"
#include "Engine/Factory/FramebufferFactory.h"
#include "Engine/Factory/VertexArrayFactory.h"
#include "Engine/Library/ShaderLibrary.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	BloomEffect::BloomEffect()
	{
		FramebufferSpecification blurFbSpec;
		blurFbSpec.attachments = FramebufferAttachmentSpecification(
			{
				FramebufferTextureFormat::RGBA16,
			});
		m_blurFramebuffers[0] = FramebufferFactory::Create(blurFbSpec);
		m_blurFramebuffers[1] = FramebufferFactory::Create(blurFbSpec);

		FramebufferSpecification finalFbSpec;
		finalFbSpec.attachments = FramebufferAttachmentSpecification(
			{
				FramebufferTextureFormat::RGBA16,
			});
		m_finalFramebuffer = FramebufferFactory::Create(finalFbSpec);

		std::vector<float> quadVertices =
		{
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		m_vertexArray = VertexArrayFactory::Create();
		Ptr<IVertexBuffer> vertexBuffer = BufferFactory::CreateVertexBuffer(&(quadVertices)[0], quadVertices.size());
		vertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3 },
				{ ShaderDataType::Float2 },
			});
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		m_shaderBlur = ShaderLibrary::Load("assets/shaders/Bloom/Blur.glsl");
		m_shaderFinal = ShaderLibrary::Load("assets/shaders/Bloom/FinalScene.glsl");
		m_shaderBlur->Bind();
		m_shaderBlur->SetInt("uScene", 0);
		m_shaderFinal->Bind();
		m_shaderFinal->SetInt("uScene", 0);
		m_shaderFinal->SetInt("uBloomBlur", 1);
	}

	void BloomEffect::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_blurFramebuffers[0]->Resize(width, height);
		m_blurFramebuffers[1]->Resize(width, height);
		m_finalFramebuffer->Resize(width, height);
	}

	void BloomEffect::OnUpdate(Ptr<IFramebuffer> sceneFramebuffer)
	{
		bool horizontal = true;
		bool firstIteration = true;
		uint32_t amount = 10;
		m_shaderBlur->Bind();
		for (uint32_t i = 0; i < amount; ++i)
		{
			m_blurFramebuffers[horizontal]->Bind();
			m_shaderBlur->SetInt("uHorizontal", horizontal);
			if (firstIteration)
			{
				sceneFramebuffer->BindColorTexture(0, 1);
			}
			else
			{
				m_blurFramebuffers[!horizontal]->BindColorTexture(0);
			}

			m_vertexArray->Bind();
			RendererCommand::DrawArraysStrip(4);
			m_vertexArray->Unbind();

			horizontal = !horizontal;
			if (firstIteration)
			{
				firstIteration = false;
			}

			m_blurFramebuffers[horizontal]->Unbind();
		}

		m_finalFramebuffer->Bind();
		m_shaderFinal->Bind();
		RendererCommand::Clear();
		sceneFramebuffer->BindColorTexture(0);
		m_blurFramebuffers[0]->BindColorTexture(1);
		m_vertexArray->Bind();
		RendererCommand::DrawArraysStrip(4);
		m_vertexArray->Unbind();
		m_finalFramebuffer->Unbind();
	}
}
