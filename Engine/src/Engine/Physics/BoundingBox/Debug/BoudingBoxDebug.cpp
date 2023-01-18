#include "enginepch.h"
#include "BoudingBoxDebug.h"
#include "BoudingBoxDebugData.h"

#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	BoudingBoxDebug::BoudingBoxDebug()
	{
		m_vertexArray = Engine::VertexArray::Create();
		Ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(const_cast<float*>(BoudingBoxDebugData::vertices), BoudingBoxDebugData::numOfVertices);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
			});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(IndexBuffer::Create(const_cast<uint8_t*>(BoudingBoxDebugData::indices), BoudingBoxDebugData::numOfIndices));

		m_shader = ShaderLibrary::GetInstance()->Load("assets/shaders/BoudingBox.glsl");
	}

	void BoudingBoxDebug::Draw(Transform& transform, BoundingValue boundingValue)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.translation + boundingValue.center) *
			glm::toMat4(glm::quat(transform.rotation)) *
			glm::scale(glm::mat4(1.0f), transform.scale * boundingValue.extents);

		PolygonMode currentMode = RendererCommand::GetPolygonMode();
		RendererCommand::SetPolygonMode(PolygonMode::LINE);
		m_shader->Bind();
		m_shader->SetMat4("uModel", model);
		m_vertexArray->Bind();
		RendererCommand::DrawUint8Indexed(BoudingBoxDebugData::numOfIndices);
		RendererCommand::SetPolygonMode(currentMode);
	}
}
