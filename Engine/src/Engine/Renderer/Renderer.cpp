#include "enginepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Uniq<Renderer::SceneData> Renderer::m_sceneData = std::make_unique<Renderer::SceneData>();

	void Renderer::Init()
	{
		RendererCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RendererCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_sceneData->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ptr<Shader>& shader, const Ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uViewProjection", m_sceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("uModel", transform);

		vertexArray->Bind();
		RendererCommand::DrawIndexed(vertexArray);
	}
}