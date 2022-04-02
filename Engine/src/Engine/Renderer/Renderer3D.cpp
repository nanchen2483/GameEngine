#include "enginepch.h"
#include "Renderer3D.h"

namespace Engine
{
	struct Renderer3DData
	{
		static const uint32_t NUM_OF_VERTICES = 8;
		static const uint32_t NUM_OF_VERTEX_INDICES = 36;

		static const uint32_t MAX_QUADS = 20000;
		static const uint32_t MAX_VERTICES = MAX_QUADS * NUM_OF_VERTICES;
		static const uint32_t MAX_INDICES = MAX_QUADS * NUM_OF_VERTEX_INDICES;
		static const uint32_t MAX_TEXTURE_SLOTS = 32;

		Ptr<VertexArray> vertexArray;
		Ptr<VertexBuffer> vertexBuffer;
		Ptr<Shader> shader;
		Ptr<Texture2D> whiteTexture;

		uint32_t indexCount = 0;
		const Vertex* vertexBufferBase = new Vertex[Renderer3DData::MAX_VERTICES];
		Vertex* vertexBufferPtr = nullptr;

		static const uint32_t NUM_OF_DEFAULT_TEXTURES = 1; // The first texture slot is white block
		std::array<Ptr<Texture2D>, MAX_TEXTURE_SLOTS> textureSlots;
		uint32_t numOfTextureSlots = NUM_OF_DEFAULT_TEXTURES;

		glm::vec4 vertexPosition[NUM_OF_VERTICES] = {};
		glm::vec2 textureCoords[NUM_OF_VERTICES] = {};
	};

	static Renderer3DData s_data;

	void Renderer3D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.vertexArray = VertexArray::Create();

		s_data.vertexBuffer = VertexBuffer::Create(Renderer3DData::MAX_VERTICES * sizeof(Vertex));
		s_data.vertexBuffer->SetLayout(BufferLayout
		{
			{ ShaderDataType::Float3,	ShaderDataName::Position },
			{ ShaderDataType::Float3,	ShaderDataName::Normal },
			{ ShaderDataType::Float4,	ShaderDataName::Color },
			{ ShaderDataType::Float2,	ShaderDataName::TexCoord },
			{ ShaderDataType::Float,	ShaderDataName::TexIndex },
			{ ShaderDataType::Float3,	ShaderDataName::Tangent },
			{ ShaderDataType::Float3,	ShaderDataName::Bitangent },
			{ ShaderDataType::Float4,	ShaderDataName::Bone1 },
			{ ShaderDataType::Float4,	ShaderDataName::Bone2 },
			{ ShaderDataType::Int4,		ShaderDataName::BoneIds },
			{ ShaderDataType::Float4,	ShaderDataName::Weights },
			{ ShaderDataType::Int,		ShaderDataName::EntityId }
		});
		s_data.vertexArray->AddVertexBuffer(s_data.vertexBuffer);

		uint32_t* indices = new uint32_t[Renderer3DData::MAX_INDICES];
		for (uint32_t i = 0, indicesOffset = 0; i < Renderer3DData::MAX_INDICES; i += Renderer3DData::NUM_OF_VERTEX_INDICES, indicesOffset += Renderer3DData::NUM_OF_VERTICES)
		{
			// Front
			indices[i + 0] = indicesOffset + 0;
			indices[i + 1] = indicesOffset + 1;
			indices[i + 2] = indicesOffset + 2;
			indices[i + 3] = indicesOffset + 2;
			indices[i + 4] = indicesOffset + 3;
			indices[i + 5] = indicesOffset + 0;
			// Back
			indices[i + 6] = indicesOffset + 4;
			indices[i + 7] = indicesOffset + 5;
			indices[i + 8] = indicesOffset + 6;
			indices[i + 9] = indicesOffset + 6;
			indices[i + 10] = indicesOffset + 7;
			indices[i + 11] = indicesOffset + 4;
			// Left
			indices[i + 12] = indicesOffset + 5;
			indices[i + 13] = indicesOffset + 0;
			indices[i + 14] = indicesOffset + 3;
			indices[i + 15] = indicesOffset + 3;
			indices[i + 16] = indicesOffset + 6;
			indices[i + 17] = indicesOffset + 5;
			// Right
			indices[i + 18] = indicesOffset + 1;
			indices[i + 19] = indicesOffset + 4;
			indices[i + 20] = indicesOffset + 7;
			indices[i + 21] = indicesOffset + 7;
			indices[i + 22] = indicesOffset + 2;
			indices[i + 23] = indicesOffset + 1;
			// Top
			indices[i + 24] = indicesOffset + 3;
			indices[i + 25] = indicesOffset + 2;
			indices[i + 26] = indicesOffset + 7;
			indices[i + 27] = indicesOffset + 7;
			indices[i + 28] = indicesOffset + 6;
			indices[i + 29] = indicesOffset + 3;
			// Bottom
			indices[i + 30] = indicesOffset + 5;
			indices[i + 31] = indicesOffset + 4;
			indices[i + 32] = indicesOffset + 1;
			indices[i + 33] = indicesOffset + 1;
			indices[i + 34] = indicesOffset + 0;
			indices[i + 35] = indicesOffset + 5;
		}

		Ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, Renderer3DData::MAX_INDICES);
		s_data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] indices;

		s_data.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[Renderer3DData::MAX_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < s_data.MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		s_data.shader = Shader::Create("asserts/shaders/Color.glsl");
		s_data.shader->Bind();
		s_data.shader->SetIntArray("uTextures", samplers, Renderer3DData::MAX_TEXTURE_SLOTS);
		s_data.textureSlots[0] = s_data.whiteTexture;

		s_data.vertexPosition[0] = { -0.5, -0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[1] = {  0.5, -0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[2] = {  0.5,  0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[3] = { -0.5,  0.5f,  0.5f, 1.0f };
		s_data.vertexPosition[4] = {  0.5, -0.5f, -0.5f, 1.0f };
		s_data.vertexPosition[5] = { -0.5, -0.5f, -0.5f, 1.0f };
		s_data.vertexPosition[6] = { -0.5,  0.5f, -0.5f, 1.0f };
		s_data.vertexPosition[7] = {  0.5,  0.5f, -0.5f, 1.0f };

		s_data.textureCoords[0] = { 0.0f, 0.0f };
		s_data.textureCoords[1] = { 1.0f, 0.0f };
		s_data.textureCoords[2] = { 1.0f, 1.0f };
		s_data.textureCoords[3] = { 0.0f, 1.0f };
		s_data.textureCoords[4] = { 1.0f, 0.0f };
		s_data.textureCoords[5] = { 0.0f, 0.0f };
		s_data.textureCoords[6] = { 0.0f, 1.0f };
		s_data.textureCoords[7] = { 1.0f, 1.0f };
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

		glm::mat4& viewProjection = camera.GetViewProjection();

		ENGINE_CORE_ASSERT(s_data.shader, "");
		s_data.shader->Bind();
		s_data.shader->SetMat4("uViewProjection", viewProjection);
		s_data.vertexArray->Bind();
		ResetRendererData();
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

		const uint32_t vertexBufferSize = (uint8_t*)s_data.vertexBufferPtr - (uint8_t*)s_data.vertexBufferBase;
		s_data.vertexBuffer->SetData(s_data.vertexBufferBase, vertexBufferSize);
		for (uint32_t i = 0; i < s_data.numOfTextureSlots; i++)
		{
			s_data.textureSlots[i]->Bind(i);
		}

		RendererCommand::DrawIndexed(s_data.indexCount);
		ResetRendererData();
	}

	void Renderer3D::ResetRendererData()
	{
		s_data.indexCount = 0;
		s_data.vertexBufferPtr = (Vertex*)s_data.vertexBufferBase;
		s_data.numOfTextureSlots = Renderer3DData::NUM_OF_DEFAULT_TEXTURES;
	}

	void Renderer3D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId)
	{
		DrawCube(transform, sprite.texture, sprite.color, entityId);
	}
	
	void Renderer3D::DrawCube(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color, int entityId)
	{
		ENGINE_PROFILE_FUNCTION();

		if (s_data.indexCount >= Renderer3DData::MAX_INDICES)
		{
			DrawAndReset();
		}

		uint32_t currentTextureIndex = GetTextureIndex(texture);
		for (size_t i = 0; i < Renderer3DData::NUM_OF_VERTICES; i++)
		{
			s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[i];
			s_data.vertexBufferPtr->color = color;
			s_data.vertexBufferPtr->texCoord = s_data.textureCoords[i];
			s_data.vertexBufferPtr->textureIndex = currentTextureIndex;
			s_data.vertexBufferPtr->entityId = entityId;
			s_data.vertexBufferPtr++;
		}

		s_data.indexCount += Renderer3DData::NUM_OF_VERTEX_INDICES;
	}

	void Renderer3D::DrawModel(const glm::mat4& transform, ModelComponent& component)
	{
		if (component.model != nullptr)
		{
			component.model->Draw();
		}
	}

	void Renderer3D::DrawAnimation(const glm::mat4& transform, SkeletonAnimationComponent& component)
	{
		if (component.model != nullptr)
		{
			s_data.shader->Bind();
			std::vector<glm::mat4> transforms = component.model->GetPoseTransforms();
			for (int i = 0; i < transforms.size(); ++i)
			{
				s_data.shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
			}

			component.model->Draw();
		}
	}

	uint32_t Renderer3D::GetTextureIndex(const Ptr<Texture2D>& texture)
	{
		uint32_t currentTextureIndex = 0;
		if (texture != nullptr)
		{
			for (uint32_t i = 1; i < s_data.numOfTextureSlots; i++)
			{
				if (*s_data.textureSlots[i].get() == *texture.get())
				{
					return i;
					break;
				}
			}

			if (currentTextureIndex == 0.0f)
			{
				currentTextureIndex = s_data.numOfTextureSlots;
				s_data.textureSlots[s_data.numOfTextureSlots] = texture;
				s_data.numOfTextureSlots++;
			}
		}

		return currentTextureIndex;
	}
	
	void Renderer3D::DrawAndReset()
	{
		EndScene();
	}
}
