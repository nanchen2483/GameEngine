#include "LightBox.h"
#include "LightBoxData.h"

#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Factory/BufferFactory.h"
#include "Engine/Factory/VertexArrayFactory.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	LightBox::LightBox()
	{
		m_vertexArray = VertexArrayFactory::Create();
		Ptr<IVertexBuffer> vertexBuffer = BufferFactory::CreateVertexBuffer(const_cast<float*>(LightBoxData::vertices), LightBoxData::numOfVertices);
		vertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3 },
			});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(BufferFactory::CreateIndexBuffer(const_cast<uint8_t*>(LightBoxData::indices), LightBoxData::numOfIndices));

		m_shader = ShaderLibrary::Load("assets/shaders/LightBox.glsl");
	}

	void LightBox::Draw(glm::mat4 model, glm::vec3 color)
	{
		m_shader->Bind();
		m_shader->SetMat4("uModel", model);
		m_shader->SetFloat3("uLightColor", color);
		m_vertexArray->Bind();
		RendererCommand::DrawUint8Indexed(LightBoxData::numOfIndices);
		m_vertexArray->Unbind();
		m_shader->Unbind();
	}
}
