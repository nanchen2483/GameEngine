#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Scene/Component.h"
#include "Camera/Camera.h"
#include "Camera/EditorCamera.h"
#include "Camera/OrthographicCamera.h"
#include "RendererAPI.h"
#include "RendererCommand.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

namespace Engine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ptr<Texture2D>& texture, const glm::vec4& color = glm::vec4(1.0f), int entityId = -1);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityId = -1);
		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityId);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
		
		struct Statistics
		{
			uint32_t drawCalls = 0;
			uint32_t quadCount = 0;

			uint32_t GetTotalVertexCount() { return quadCount * 4; }
			uint32_t GetTotalIndexCount() { return quadCount * 6; }
		};
		static void ResetStates();
		static Statistics GetState();

	private:
		static void StartBatch();
		static void FlushAndReset();
	};
}
