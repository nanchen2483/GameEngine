#include "enginepch.h"
#include "Renderer3D.h"
#include "RendererCommand.h"
#include "Model/Vertex.h"
#include "Shadow/ShadowBox.h"
#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Library/TextureLibrary.h"

#include <array>
#include <glm/gtc/type_ptr.hpp>

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
		Ptr<UniformBuffer> cameraUniformBuffer;
		Ptr<UniformBuffer> dirLightUniformBuffer;
		Ptr<UniformBuffer> pointLightUniformBuffer;
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

		Renderer3D::Statistics states;

		ShadowBoxInfo shadowInfo;
	};

	static Renderer3DData s_data;

	void Renderer3D::Init()
	{
		ENGINE_PROFILE_FUNCTION();

		s_data.vertexArray = VertexArray::Create();

		s_data.vertexBuffer = VertexBuffer::Create(Renderer3DData::MAX_VERTICES * sizeof(Vertex));
		s_data.vertexBuffer->SetLayout(Vertex::GetBufferLayout());
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

		s_data.whiteTexture = TextureLibrary::GetInstance()->Load(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_data.whiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[Renderer3DData::MAX_TEXTURE_SLOTS];
		for (uint32_t i = 0; i < s_data.MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		s_data.shader = ShaderLibrary::GetInstance()->Load("assets/shaders/Default.glsl");
		s_data.shader->Bind();
		s_data.textureSlots[0] = s_data.whiteTexture;
		s_data.shader->SetIntArray("uTextures", samplers, Renderer3DData::MAX_TEXTURE_SLOTS);
		s_data.shader->SetInt("uShadowMap", s_data.shadowInfo.depthTextureSlot);
		
		s_data.shader->SetInt("uCascadeCount", s_data.shadowInfo.levels.size());
		for (size_t i = 0; i < s_data.shadowInfo.levels.size(); ++i)
		{
			s_data.shader->SetFloat("uCascadePlaneDistances[" + std::to_string(i) + "]", s_data.shadowInfo.levels[i]);
		}
		
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

		s_data.cameraUniformBuffer = UniformBuffer::Create(0, {
				BufferLayoutType::Std140,
				{
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Float3 },
				}
			});

		s_data.dirLightUniformBuffer = UniformBuffer::Create(1, {
				BufferLayoutType::Std140,
				{
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
				}
			});

		s_data.dirLightUniformBuffer->SetData({
			glm::value_ptr(glm::normalize(glm::vec3(0.2f, 1.0f, 0.3f))),
			glm::value_ptr(glm::vec3(0.05f)),
			glm::value_ptr(glm::vec3(0.4f)),
			glm::value_ptr(glm::vec3(0.5f)),
		});

		s_data.pointLightUniformBuffer = UniformBuffer::Create(2, {
				BufferLayoutType::Std140,
				{
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float },
					{ ShaderDataType::Float },
					{ ShaderDataType::Float },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
				}
			});
	}
	
	void Renderer3D::Shutdown()
	{
	}
	
	void Renderer3D::OnWindowResize(uint32_t width, uint32_t height)
	{
	}
	
	void Renderer3D::BeginScene(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjection, glm::vec3 cameraPosition, uint32_t numOfPointLights)
	{
		ENGINE_PROFILE_FUNCTION();
		ENGINE_CORE_ASSERT(s_data.shader, "Shader is null");

		s_data.shader->Bind();
		s_data.shader->SetBool("uHasPointLight", numOfPointLights > 0);
		s_data.shader->SetBool("uHasAnimation", false);
		s_data.shader->SetInt("uEntityId", -1);
		s_data.cameraUniformBuffer->SetData({ &cameraViewMatrix, &cameraProjection, &cameraPosition });
		s_data.vertexArray->Bind();
		ResetRendererData();
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

		RendererCommand::DrawUint32Indexed(s_data.indexCount);

		s_data.states.drawCalls++;
		ResetRendererData();
	}

	void Renderer3D::ResetRendererData()
	{
		s_data.indexCount = 0;
		s_data.vertexBufferPtr = (Vertex*)s_data.vertexBufferBase;
		s_data.numOfTextureSlots = Renderer3DData::NUM_OF_DEFAULT_TEXTURES;
	}

	void Renderer3D::Draw(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId)
	{
		Draw(transform, sprite.texture, sprite.color, entityId);
	}

	void Renderer3D::Draw(const Transform& transform, LightComponent& light, int entityId)
	{
		light.position = transform.translation;
		s_data.pointLightUniformBuffer->SetData(light.GetData());
		Draw(transform, nullptr, glm::vec4(1.0f), entityId);
	}
	
	void Renderer3D::Draw(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color, int entityId)
	{
		ENGINE_PROFILE_FUNCTION();

		if (s_data.indexCount >= Renderer3DData::MAX_INDICES)
		{
			DrawAndReset();
		}

		glm::mat3 inverseTransform = glm::transpose(glm::inverse(glm::mat3(transform)));
		uint32_t currentTextureIndex = GetTextureIndex(texture);
		for (size_t i = 0; i < Renderer3DData::NUM_OF_VERTICES; i++)
		{
			s_data.vertexBufferPtr->position = transform * s_data.vertexPosition[i];
			s_data.vertexBufferPtr->normal = inverseTransform * s_data.vertexPosition[i];
			s_data.vertexBufferPtr->color = color;
			s_data.vertexBufferPtr->texCoord = s_data.textureCoords[i];
			s_data.vertexBufferPtr->material = glm::vec3(currentTextureIndex, -1, 0);
			s_data.vertexBufferPtr->isWorldPos = true;
			s_data.vertexBufferPtr->entityId = entityId;
			s_data.vertexBufferPtr++;
		}

		s_data.indexCount += Renderer3DData::NUM_OF_VERTEX_INDICES;
	}

	void Renderer3D::Draw(const glm::mat4& transform, std::vector<Ptr<Mesh>> meshes, Ptr<Animation> animation, Ptr<Shader> shader, int entityId)
	{
		if (!meshes.empty())
		{
			if (shader == nullptr)
			{
				// Use default shader
				shader = s_data.shader;
				shader->SetMat3("uInverseModel", glm::transpose(glm::inverse(glm::mat3(transform))));
				shader->SetInt("uEntityId", entityId);
			}

			shader->SetMat4("uModel", transform);
			shader->SetBool("uHasAnimation", animation != nullptr);
			if (animation != nullptr)
			{
				std::vector<glm::mat4> transforms = animation->GetBoneTransforms();
				for (uint32_t i = 0; i < transforms.size(); i++)
				{
					shader->SetMat4("uBoneTransforms[" + std::to_string(i) + "]", transforms[i]);
				}
			}

			for (uint32_t i = 0; i < meshes.size(); i++)
			{
				meshes[i]->GetMaterial()->Bind();
				meshes[i]->GetVertexArray()->Bind();
				RendererCommand::DrawUint32Indexed(meshes[i]->GetVertexArray()->GetNumOfIndices());
			}

			s_data.states.drawModels++;
		}
	}

	void Renderer3D::Draw(const glm::mat4& transform, TerrainComponent& component, const Frustum& frustum)
	{
		if (component.terrain != nullptr)
		{
			component.terrain->Draw(transform, frustum);
		}
	}

	void Renderer3D::Draw(SkyboxComponent& component)
	{
		if (component.skybox != nullptr)
		{
			component.skybox->Draw();
		}
	}

	void Renderer3D::ResetStates()
	{
		s_data.states.drawCalls = 0;
		s_data.states.drawModels = 0;
	}

	Renderer3D::Statistics Renderer3D::GetState()
	{
		return s_data.states;
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
