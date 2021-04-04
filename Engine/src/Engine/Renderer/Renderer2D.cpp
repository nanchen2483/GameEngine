#include "enginepch.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	struct Renderer2DStorage
	{
		Ptr<VertexArray> vertexArray;
		Ptr<Shader> shader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::Init()
	{
		s_data = new Renderer2DStorage();
		s_data->vertexArray = VertexArray::Create();

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		Ptr<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			});
		s_data->vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ptr<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_data->vertexArray->SetIndexBuffer(indexBuffer);

		s_data->shader = Shader::Create("asserts/shaders/Color.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_data;
	}
	
	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
	}
	
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_data->shader->Bind();
		s_data->shader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());
	}
	
	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_data->shader->Bind();
		s_data->shader->SetMat4("uModel", glm::translate(glm::mat4(1.0f), position));
		s_data->shader->SetFloat4("uColor", color);
		s_data->vertexArray->Bind();
		RendererCommand::DrawIndexed(s_data->vertexArray);
	}
}
