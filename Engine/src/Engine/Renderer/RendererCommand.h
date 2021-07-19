#pragma once

#include "RendererAPI.h"

namespace Engine
{
	class RendererCommand
	{
	public:
		inline static void Init()
		{
			s_rendererAPI->Init();
		}

		inline static void  SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_rendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->DrawIndexed(vertexArray);
		}

		inline static void DrawIndexed(uint32_t count)
		{
			s_rendererAPI->DrawIndexed(count);
		}
	private:
		static RendererAPI* s_rendererAPI;
	};
}
