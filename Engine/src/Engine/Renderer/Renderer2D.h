#pragma once

#include "Engine/Core/Base.h"
#include "RendererAPI.h"

#include "RendererCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include "Texture.h"

namespace Engine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(const glm::mat4& projection, const glm::mat4& transform);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ptr<Texture2D>& texture);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);

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
		static void FlushAndReset();
	};
}