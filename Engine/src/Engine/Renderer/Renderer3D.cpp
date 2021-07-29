#include "enginepch.h"
#include "Renderer3D.h"

namespace Engine
{
	struct Renderer3DData
	{
		static const uint32_t numOfVertices = 8;
		static const uint32_t numOfVertexIndices = 36;

		static const uint32_t maxQuads = 20000;
		static const uint32_t maxVertices = maxQuads * numOfVertices;
		static const uint32_t maxIndices = maxQuads * numOfVertexIndices;
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

		glm::vec4 vertexPosition[numOfVertices] = {};
		glm::vec2 textureCoords[numOfVertices] = {};
	};

	static Renderer3DData s_data;

	void Renderer3D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.vertexArray = VertexArray::Create();

		s_data.vertexBuffer = VertexBuffer::Create(s_data.maxVertices * sizeof(Vertex));
		s_data.vertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPosition" },
			{ ShaderDataType::Float4, "aColor" },
			{ ShaderDataType::Float2, "aTexCoord" },
			{ ShaderDataType::Float, "aTexIndex" },
			{ ShaderDataType::Int, "aEntityId" }
			});
		s_data.vertexArray->AddVertexBuffer(s_data.vertexBuffer);

		s_data.vertexBufferBase = new Vertex[s_data.maxVertices];

		uint32_t* indices = new uint32_t[s_data.maxIndices];
		uint32_t indicesOffset = 0;
		for (uint32_t i = 0; i < s_data.maxIndices; i += s_data.numOfVertexIndices)
		{
			indices[i + 0] = indicesOffset + 0;
			indices[i + 1] = indicesOffset + 1;
			indices[i + 2] = indicesOffset + 2;
			indices[i + 3] = indicesOffset + 2;
			indices[i + 4] = indicesOffset + 3;
			indices[i + 5] = indicesOffset + 0;

			indices[i + 6] = indicesOffset + 4;
			indices[i + 7] = indicesOffset + 5;
			indices[i + 8] = indicesOffset + 6;
			indices[i + 9] = indicesOffset + 5;
			indices[i + 10] = indicesOffset + 4;
			indices[i + 11] = indicesOffset + 7;

			indices[i + 12] = indicesOffset + 3;
			indices[i + 13] = indicesOffset + 7;
			indices[i + 14] = indicesOffset + 4;
			indices[i + 15] = indicesOffset + 4;
			indices[i + 16] = indicesOffset + 0;
			indices[i + 17] = indicesOffset + 3;

			indices[i + 18] = indicesOffset + 2;
			indices[i + 19] = indicesOffset + 6;
			indices[i + 20] = indicesOffset + 5;
			indices[i + 21] = indicesOffset + 6;
			indices[i + 22] = indicesOffset + 2;
			indices[i + 23] = indicesOffset + 1;

			indices[i + 24] = indicesOffset + 4;
			indices[i + 25] = indicesOffset + 6;
			indices[i + 26] = indicesOffset + 1;
			indices[i + 27] = indicesOffset + 1;
			indices[i + 28] = indicesOffset + 0;
			indices[i + 29] = indicesOffset + 4;

			indices[i + 30] = indicesOffset + 7;
			indices[i + 31] = indicesOffset + 2;
			indices[i + 32] = indicesOffset + 5;
			indices[i + 33] = indicesOffset + 2;
			indices[i + 34] = indicesOffset + 7;
			indices[i + 35] = indicesOffset + 3;

			indicesOffset += s_data.numOfVertices;
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

		s_data.vertexPosition[0] = { -0.5, -0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[1] = {  0.5, -0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[2] = {  0.5,  0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[3] = { -0.5,  0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[4] = { -0.5, -0.5f, -0.5f, 1.0f };
		s_data.vertexPosition[5] = {  0.5,  0.5f, -0.5f, 1.0f };
		s_data.vertexPosition[6] = {  0.5, -0.5f, -0.5f, 1.0f };
		s_data.vertexPosition[7] = { -0.5,  0.5f, -0.5f, 1.0f };

		s_data.textureCoords[0] = { 0.0f, 0.0f };
		s_data.textureCoords[1] = { 1.0f, 0.0f };
		s_data.textureCoords[2] = { 1.0f, 1.0f };
		s_data.textureCoords[3] = { 0.0f, 1.0f };
		s_data.textureCoords[4] = { 0.0f, 0.0f };
		s_data.textureCoords[5] = { 1.0f, 1.0f };
		s_data.textureCoords[6] = { 1.0f, 0.0f };
		s_data.textureCoords[7] = { 0.0f, 1.0f };
	}
	
	void Renderer3D::Shutdown()
	{
	}
	
	void Renderer3D::OnWindowResize(uint32_t width, uint32_t height)
	{
	}
	
	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		ENGINE_PROFILE_FUNCTION();

		auto& viewProjection = camera.GetViewProjection();

		ENGINE_CORE_ASSERT(s_data.shader, "");
		s_data.shader->Bind();
		s_data.shader->SetMat4("uViewProjection", viewProjection);

		StartBatch();
	}
	
	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
	}
	
	void Renderer3D::BeginScene(OrthographicCamera& camera)
	{
	}
	
	void Renderer3D::EndScene()
	{
		ENGINE_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_data.vertexBufferPtr - (uint8_t*)s_data.vertexBufferBase;
		s_data.vertexBuffer->SetData(s_data.vertexBufferBase, dataSize);
		Flush();
	}
	
	void Renderer3D::Flush()
	{
		for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
		{
			s_data.textureSlots[i]->Bind(i);
		}

		RendererCommand::DrawIndexed(s_data.indexCount);
	}
	
	void Renderer3D::DrawCube(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color, int entityId)
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

		for (size_t i = 0; i < s_data.numOfVertices; i++)
		{
			s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[i];
			s_data.vertexBufferPtr->color = color;
			s_data.vertexBufferPtr->texCoord = s_data.textureCoords[i];
			s_data.vertexBufferPtr->textureIndex = textureIndex;
			s_data.vertexBufferPtr->entityId = entityId;
			s_data.vertexBufferPtr++;
		}

		s_data.indexCount += s_data.numOfVertexIndices;
	}
	
	void Renderer3D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId)
	{
		DrawCube(transform, sprite.texture, sprite.color, entityId);
	}
	
	void Renderer3D::StartBatch()
	{
		s_data.indexCount = 0;
		s_data.vertexBufferPtr = s_data.vertexBufferBase;
		s_data.textureSlotIndex = 1;
	}
	
	void Renderer3D::FlushAndReset()
	{
		EndScene();
		StartBatch();
	}
}
