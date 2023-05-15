#include "enginepch.h"
#include "Skybox.h"
#include "SkyboxData.h"

#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	Skybox::Skybox(const std::vector<Ptr<Image>> faces)
		: Skybox(ITexture3D::Create(faces))
	{
	}

	Skybox::Skybox(Ptr<ITexture3D> texture)
		: m_texture(texture)
	{
		m_vertexArray = Engine::IVertexArray::Create();
		Ptr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(const_cast<float*>(SkyboxData::vertices), SkyboxData::numOfVertices);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(IIndexBuffer::Create(const_cast<uint8_t*>(SkyboxData::indices), SkyboxData::numOfIndices));

		m_shader = ShaderLibrary::Load("assets/shaders/Skybox.glsl");
	}
	
	void Skybox::Draw()
	{
		RendererCommand::SetDepthFunc(DepthFunc::Lequal);
		RendererCommand::CullFace(FaceCulling::Back);
		m_shader->Bind();
		m_vertexArray->Bind();
		m_texture->Bind();
		RendererCommand::CullFace(FaceCulling::Front);
		RendererCommand::DrawUint8Indexed(SkyboxData::numOfIndices);
		RendererCommand::SetDepthFunc(DepthFunc::Less); // Set to default
	}
}
