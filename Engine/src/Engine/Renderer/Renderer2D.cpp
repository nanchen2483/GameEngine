#include "enginepch.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{	
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 texCoord;
	};

	struct Renderer2DData
	{
		const uint32_t maxQuads = 10000;
		const uint32_t maxVertices = maxQuads * 4;
		const uint32_t maxIndices = maxQuads * 6;
		
		Ptr<VertexArray> vertexArray;
		Ptr<VertexBuffer> vertexBuffer;
		Ptr<Shader> shader;
		Ptr<Texture2D> whiteTexture;

		uint32_t indexCount = 0;
		Vertex* vertexBufferBase = nullptr;
		Vertex* vertexBufferPtr = nullptr;
	};

	static Renderer2DData s_data;

	void Renderer2D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.vertexArray = VertexArray::Create();

		s_data.vertexBuffer = VertexBuffer::Create(s_data.maxVertices * sizeof(Vertex));
		s_data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float2, "aTexCoord" },
		});
		s_data.vertexArray->AddVertexBuffer(s_data.vertexBuffer);

		s_data.vertexBufferBase = new Vertex[s_data.maxVertices];

		uint32_t* indices = new uint32_t[s_data.maxIndices];
		uint32_t indicesOffset = 0;
		for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
		{
			indices[i + 0] = indicesOffset + 0;
			indices[i + 1] = indicesOffset + 1;
			indices[i + 2] = indicesOffset + 2;

			indices[i + 3] = indicesOffset + 2;
			indices[i + 4] = indicesOffset + 3;
			indices[i + 5] = indicesOffset + 0;

			indicesOffset += 4;
		}
		Ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, s_data.maxIndices);
		s_data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		s_data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		s_data.shader = Shader::Create("asserts/shaders/Color.glsl");
		s_data.shader->Bind();
		s_data.shader->SetInt("uTexture", 0);
	}

	void Renderer2D::Shutdown()
	{
	}
	
	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
	}
	
	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.shader->Bind();
		s_data.shader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

		s_data.indexCount = 0;
		s_data.vertexBufferPtr = s_data.vertexBufferBase;
	}
	
	void Renderer2D::EndScene()
	{
		ENGINE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t *)s_data.vertexBufferPtr - (uint8_t*)s_data.vertexBufferBase;
		s_data.vertexBuffer->SetData(s_data.vertexBufferBase, dataSize);
		Flush();
	}

	void Renderer2D::Flush()
	{
		RendererCommand::DrawIndexed(s_data.indexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		ENGINE_PROFILE_FUNCTION();

		auto pos = glm::vec3((position.x + size.x) /2.0f,(position.y + size.y) / 2.0f, 0.0f);
		s_data.vertexBufferPtr->position = position - pos;
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = glm::vec3(position.x + size.x, position.y, 0.0f) - pos;
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = glm::vec3(position.x + size.x, position.y + size.y, 0.0f) - pos;
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = glm::vec3(position.x, position.y + size.y, 0.0f) - pos;
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_data.vertexBufferPtr++;

		s_data.indexCount += 6;

		/*s_data.shader->Bind();
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_data.shader->SetMat4("uModel", transform);
		s_data.shader->SetFloat4("uColor", color);
		s_data.vertexArray->Bind();
		s_data.whiteTexture->Bind();
		RendererCommand::DrawIndexed(s_data.vertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ptr<Texture>& texture)
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.shader->Bind();
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_data.shader->SetMat4("uModel", transform);
		s_data.shader->SetFloat4("uColor", glm::vec4(1.0f));
		s_data.vertexArray->Bind();
		texture->Bind();
		RendererCommand::DrawIndexed(s_data.vertexArray);
	}
}
