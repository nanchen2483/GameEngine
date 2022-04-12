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
		static void BeginScene(const EditorCamera& camera, uint32_t numOfPointLights = 0);
		static void BeginScene(const Camera& camera, const TransformComponent& transform, uint32_t numOfPointLights = 0);
		static void BeginScene(OrthographicCamera& camera);
		
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId = -1);
		static void DrawLight(const TransformComponent& transform, LightComponent& light, int entityId = -1);
		static void DrawCube(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), int entityId = -1);
		static void DrawModel(const glm::mat4& transform, ModelComponent& component, const float deltaTime);

		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	private:
		static uint32_t GetTextureIndex(const Ptr<Texture2D>& texture);
		static void DrawAndReset();
		static void ResetRendererData();
	};
}
