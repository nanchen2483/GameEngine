#pragma once
#include "Engine/Renderer/Model/Animation/IAnimation.h"
#include "Engine/Renderer/Model/Mesh/IMesh.h"
#include "Engine/Scene/Component/LightComponent.h"
#include "Engine/Scene/Component/SkyboxComponent.h"
#include "Engine/Scene/Component/SpriteRendererComponent.h"
#include "Engine/Scene/Component/TerrainComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"
#include "Engine/Renderer/Camera/OrthographicCamera.h"

namespace Engine
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjection, glm::vec3 cameraPosition, uint32_t numOfPointLights = 0);
		
		static void Draw(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId = -1);
		static void Draw(const Transform& transform, LightComponent& light, int entityId = -1);
		static void Draw(const glm::mat4& transform, const Ptr<ITexture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), int entityId = -1);
		static void Draw(const glm::mat4& transform, std::vector<Ptr<IMesh>> meshes, Ptr<IAnimation> animation = nullptr, Ptr<IShader> shader = nullptr, int entityId = -1);
		static void Draw(const glm::mat4& transform, TerrainComponent& component, const Frustum& frustum);
		static void Draw(SkyboxComponent& component);

		static void EndScene();

		struct Statistics
		{
			uint32_t drawCalls{};
			uint32_t drawModels{};
		};
		static void ResetStates();
		static Statistics GetState();
	private:
		static uint32_t GetTextureIndex(const Ptr<ITexture2D>& texture);
		static void DrawAndReset();
		static void ResetRendererData();
	};
}
