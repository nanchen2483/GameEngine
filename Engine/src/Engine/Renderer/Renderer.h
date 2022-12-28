#pragma once
#include "Engine/Core/Base.h"
#include "RendererAPI.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	};
}
