#include "enginepch.h"
#include "Skybox.h"
#include "SkyboxData.h"

#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	Skybox::Skybox(const std::vector<Ptr<Image>> faces)
		: Skybox(Texture3D::Create(faces))
	{
	}

	Skybox::Skybox(Ptr<Texture3D> texture)
		: m_texture(texture)
	{
		m_vertexArray = Engine::VertexArray::Create();
		Ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(const_cast<float*>(SkyboxData::vertices), SkyboxData::numOfVertices);
		vertexBuffer->SetLayout({
			{ Engine::ShaderDataType::Float3 },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(IndexBuffer::Create(const_cast<uint32_t*>(SkyboxData::indices), SkyboxData::numOfIndices));

		m_shader = Shader::Create("asserts\\shaders\\Skybox.glsl");
	}
	
	void Skybox::Draw()
	{
		RendererCommand::SetDepthFunc(DepthFunc::LEQUAL);
		m_shader->Bind();
		m_vertexArray->Bind();
		m_texture->Bind();
		RendererCommand::DrawIndexed(SkyboxData::numOfIndices);
		RendererCommand::SetDepthFunc(DepthFunc::LESS); // Set to default
	}
}
