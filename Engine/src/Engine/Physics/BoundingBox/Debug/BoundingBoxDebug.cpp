#include "enginepch.h"
#include "BoundingBoxDebug.h"
#include "BoundingBoxDebugData.h"

#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	BoundingBoxDebug::BoundingBoxDebug()
	{
		m_vertexArray = IVertexArray::Create();
		Ptr<IVertexBuffer> vertexBuffer = IVertexBuffer::Create(const_cast<float*>(BoundingBoxDebugData::vertices), BoundingBoxDebugData::numOfVertices);
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
			});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(IIndexBuffer::Create(const_cast<uint8_t*>(BoundingBoxDebugData::indices), BoundingBoxDebugData::numOfIndices));

		m_shader = ShaderLibrary::Load("assets/shaders/BoundingBox.glsl");
	}

	void BoundingBoxDebug::Draw(Transform& transform, BoundingValue boundingValue)
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.translation + boundingValue.center) *
			glm::toMat4(glm::quat(transform.rotation)) *
			glm::scale(glm::mat4(1.0f), transform.scale * boundingValue.extents);

		PolygonMode currentMode = RendererCommand::GetPolygonMode();
		RendererCommand::SetPolygonMode(PolygonMode::Line);
		m_shader->Bind();
		m_shader->SetMat4("uModel", model);
		m_vertexArray->Bind();
		RendererCommand::DrawUint8Indexed(BoundingBoxDebugData::numOfIndices);
		RendererCommand::SetPolygonMode(currentMode);
	}
}
