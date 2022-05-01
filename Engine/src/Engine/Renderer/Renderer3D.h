#pragma once
#include <Engine.h>

namespace Engine
{
	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(glm::mat4 cameraViewMatrix, glm::mat4 cameraProjection, glm::vec3 cameraPosition, uint32_t numOfPointLights = 0);
		static void BeginScene(OrthographicCamera& camera);
		
		static void Draw(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId = -1);
		static void Draw(const TransformComponent& transform, LightComponent& light, int entityId = -1);
		static void Draw(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), int entityId = -1);
		static void Draw(const glm::mat4& transform, ModelComponent& component);
		static void Draw(SkyboxComponent& component);

		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t drawModels = 0;
		};
		static void ResetStates();
		static Statistics GetState();
	private:
		static uint32_t GetTextureIndex(const Ptr<Texture2D>& texture);
		static void DrawAndReset();
		static void ResetRendererData();
	};
}
