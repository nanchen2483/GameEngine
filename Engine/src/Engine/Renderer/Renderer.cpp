#include "enginepch.h"
#include "Renderer.h"

namespace Engine
{
	Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("uViewProjection", m_sceneData->viewProjectionMatrix);
		shader->UploadUniformMat4("uModel", transform);

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}