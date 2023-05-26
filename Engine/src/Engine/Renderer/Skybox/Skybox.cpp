#include "enginepch.h"
#include "Skybox.h"
#include "SkyboxData.h"

#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Factory/BufferFactory.h"
#include "Engine/Factory/TextureFactory.h"
#include "Engine/Factory/VertexArrayFactory.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	Skybox::Skybox(const std::vector<Ptr<Image>> faces)
		: Skybox(TextureFactory::Create3D(faces))
	{
	}

	Skybox::Skybox(Ptr<ITexture3D> texture)
		: m_texture(texture)
	{
		m_vertexArray = VertexArrayFactory::Create();
		Ptr<IVertexBuffer> vertexBuffer = BufferFactory::CreateVertexBuffer(const_cast<float*>(SkyboxData::vertices), SkyboxData::numOfVertices);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(BufferFactory::CreateIndexBuffer(const_cast<uint8_t*>(SkyboxData::indices), SkyboxData::numOfIndices));

		m_shader = ShaderLibrary::Load("assets/shaders/Skybox.glsl");
	}
	
	void Skybox::Draw()
	{
		RendererCommand::SetDepthFunc(DepthFunc::Lequal);
		RendererCommand::CullFace(FaceCulling::Front);
		m_shader->Bind();
		m_vertexArray->Bind();
		m_texture->Bind();
		RendererCommand::DrawUint8Indexed(SkyboxData::numOfIndices);
		RendererCommand::CullFace(FaceCulling::Back);
		RendererCommand::SetDepthFunc(DepthFunc::Less); // Set to default
	}
}
