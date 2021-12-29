#include "enginepch.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	struct Renderer2DData
	{
		static const uint32_t maxQuads = 20000;
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

		glm::vec4 vertexPosition[4] = {};

		Renderer2D::Statistics states;
	};

	static Renderer2DData s_data;

	void Renderer2D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.vertexArray = VertexArray::Create();

		s_data.vertexBuffer = VertexBuffer::Create(s_data.maxVertices * sizeof(Vertex));
		s_data.vertexBuffer->SetLayout(BufferLayout
		{
			{ ShaderDataType::Float3,  ShaderDataName::Position },
			{ ShaderDataType::Float4,  ShaderDataName::Color },
			{ ShaderDataType::Float2,  ShaderDataName::TexCoord },
			{ ShaderDataType::Float,  ShaderDataName::TexIndex },
			{ ShaderDataType::Int,  ShaderDataName::EntityId }
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

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		ENGINE_PROFILE_FUNCTION();

		auto& viewProjection = camera.GetViewProjection();

		s_data.shader->Bind();
		s_data.shader->SetMat4("uViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.shader->Bind();
		s_data.shader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		ENGINE_PROFILE_FUNCTION();

		auto& viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_data.shader->Bind();
		s_data.shader->SetMat4("uViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer2D::StartBatch()
	{
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
		
		s_data.states.drawCalls++;
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();
		StartBatch();
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		
		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color, int entityId)
	{
		ENGINE_PROFILE_FUNCTION();

		if (s_data.indexCount >= s_data.maxIndices)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		if (texture != nullptr)
		{
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
		}

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { glm::vec2(0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f), glm::vec2(0.0f, 1.0f) };

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[i];
			s_data.vertexBufferPtr->color = color;
			s_data.vertexBufferPtr->texCoord = textureCoords[i];
			s_data.vertexBufferPtr->textureIndex = textureIndex;
			s_data.vertexBufferPtr->entityId = entityId;
			s_data.vertexBufferPtr++;
		}

		s_data.indexCount += 6;

		s_data.states.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId)
	{
		ENGINE_PROFILE_FUNCTION();

		if (s_data.indexCount >= s_data.maxIndices)
		{
			FlushAndReset();
		}

		constexpr size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f;
		constexpr glm::vec2 textureCoords[] = { glm::vec2(0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f), glm::vec2(0.0f, 1.0f) };

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[i];
			s_data.vertexBufferPtr->color = color;
			s_data.vertexBufferPtr->texCoord = textureCoords[i];
			s_data.vertexBufferPtr->textureIndex = textureIndex;
			s_data.vertexBufferPtr->entityId = entityId;
			s_data.vertexBufferPtr++;
		}

		s_data.indexCount += 6;

		s_data.states.quadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId)
	{
		DrawQuad(transform, sprite.texture, sprite.color, entityId);
	}

	void Renderer2D::ResetStates()
	{
		memset(&s_data.states, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetState()
	{
		return s_data.states;
	}
}
