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
		float textureIndex;
	};

	struct Renderer2DData
	{
		static const uint32_t maxQuads = 10000;
		static const uint32_t maxVertices = maxQuads * 4;
		static const uint32_t maxIndices = maxQuads * 6;
		static const uint32_t maxTextureSlots = 32;

		Ptr<VertexArray> vertexArray;
		Ptr<VertexBuffer> vertexBuffer;
		Ptr<Shader> shader;
		Ptr<Texture2D> whiteTexture;

		uint32_t indexCount = 0;
		Vertex* vertexBufferBase = nullptr;
		Vertex* vertexBufferPtr = nullptr;

		std::array<Ptr<Texture2D>, maxTextureSlots> textureSlots;
		uint32_t textureSlotIndex = 1;

		glm::vec4 vertexPosition[4];
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
			{ ShaderDataType::Float, "aTexIndex" },
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

		int32_t samplers[s_data.maxTextureSlots];
		for (uint32_t i = 0; i < s_data.maxTextureSlots; i++)
		{
			samplers[i] = i;
		}

		s_data.shader = Shader::Create("asserts/shaders/Color.glsl");
		s_data.shader->Bind();
		s_data.shader->SetIntArray("uTextures", samplers, s_data.maxTextureSlots);
		s_data.textureSlots[0] = s_data.whiteTexture;

		s_data.vertexPosition[0] = { -0.5, -0.5f, 0.0f, 1.0f };
		s_data.vertexPosition[1] = {  0.5, -0.5f, 0.0f, 1.0f };
		s_data.vertexPosition[2] = {  0.5,  0.5f, 0.0f, 1.0f };
		s_data.vertexPosition[3] = { -0.5,  0.5f, 0.0f, 1.0f };
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
		s_data.textureSlotIndex = 1;
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
		for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
		{
			s_data.textureSlots[i]->Bind(i);
		}

		RendererCommand::DrawIndexed(s_data.indexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		ENGINE_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[0];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[1];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[2];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[3];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.indexCount += 6;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ptr<Texture2D>& texture)
	{
		ENGINE_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
		{
			if (*s_data.textureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = s_data.textureSlotIndex;
			s_data.textureSlots[s_data.textureSlotIndex] = texture;
			s_data.textureSlotIndex++;
		}

		constexpr glm::vec4 color = glm::uvec4(1.0f);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[0];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 0.0f, 0.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[1];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 1.0f, 0.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[2];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 1.0f, 1.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[3];
		s_data.vertexBufferPtr->color = color;
		s_data.vertexBufferPtr->texCoord = { 0.0f, 1.0f };
		s_data.vertexBufferPtr->textureIndex = textureIndex;
		s_data.vertexBufferPtr++;

		s_data.indexCount += 6;
	}
}
