#include "enginepch.h"
#include "ShadowDebug.h"
#include "ShadowDebugData.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	ShadowDebug::ShadowDebug()
	{
		FramebufferSpecification fbSpec;
		fbSpec.attachments = FramebufferAttachmentSpecification(
			{
				FramebufferTextureFormat::RGBA8,
			});
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::Create(fbSpec);

		m_vertexArray = Engine::VertexArray::Create();
		Ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(const_cast<float*>(ShadowDebugData::vertices), ShadowDebugData::numOfVertices);
		vertexBuffer->SetLayout({
			{ Engine::ShaderDataType::Float3 },
			{ Engine::ShaderDataType::Float2 },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(IndexBuffer::Create(const_cast<uint8_t*>(ShadowDebugData::indices), ShadowDebugData::numOfIndices));

		m_shader = Shader::Create("asserts/shaders/Debug/ShadowDebug.glsl");
		m_shader->Bind();
		m_shader->SetInt("uDepthMap", m_shadowInfo.depthTextureSlot);
		m_shader->SetInt("uLayer", 0);
	}

	void ShadowDebug::Draw()
	{
		m_framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();
		m_shader->Bind();
		m_vertexArray->Bind();
		RendererCommand::DrawUint8Indexed(ShadowDebugData::numOfIndices);
		m_framebuffer->Unbind();
	}

	uint32_t ShadowDebug::GetShadowLayer(uint32_t layer) const
	{
		m_shader->SetInt("uLayer", layer);
		return m_framebuffer->GetColorAttachmentRendererId(0);
	}
}
