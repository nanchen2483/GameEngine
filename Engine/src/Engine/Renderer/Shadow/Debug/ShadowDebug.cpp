#include "enginepch.h"
#include "ShadowDebug.h"
#include "ShadowDebugData.h"
#include "Engine/Renderer/Shadow/ShadowBoxData.h"

#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Factory/BufferFactory.h"
#include "Engine/Factory/FramebufferFactory.h"
#include "Engine/Factory/VertexArrayFactory.h"
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
		m_framebuffer = FramebufferFactory::Create(fbSpec);

		m_vertexArray = VertexArrayFactory::Create();
		Ptr<IVertexBuffer> vertexBuffer = BufferFactory::CreateVertexBuffer(const_cast<float*>(ShadowDebugData::vertices), ShadowDebugData::numOfVertices);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
			{ ShaderDataType::Float2 },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(BufferFactory::CreateIndexBuffer(const_cast<uint8_t*>(ShadowDebugData::indices), ShadowDebugData::numOfIndices));

		m_shader = ShaderLibrary::Load("assets/shaders/Debug/ShadowDebug.glsl");
		m_shader->Bind();
		m_shader->SetInt("uDepthMap", ShadowBoxData::depthTextureSlot);
		m_shader->SetInt("uLayer", 0);
	}

	void ShadowDebug::Draw()
	{
		m_framebuffer->Bind();
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
