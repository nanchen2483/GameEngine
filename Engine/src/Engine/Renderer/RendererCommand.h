#pragma once

#include "RendererAPI.h"
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"

namespace Engine
{
	class RendererCommand
	{
	public:
		inline static void Init()
		{
			s_rendererAPI->Init();
		}

		inline static void CullFace(FaceCulling face)
		{
			s_rendererAPI->CullFace(face);
		}

		inline static void  SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetDepthFunc(DepthFunc func)
		{
			s_rendererAPI->SetDepthFunc(func);
		}

		inline static void SetPolygonMode(PolygonMode mode)
		{
			s_rendererAPI->SetPolygonMode(mode);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_rendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_rendererAPI->Clear();
		}

		inline static void DrawUint8Indexed(uint32_t count)
		{
			s_rendererAPI->DrawUint8Indexed(count);
		}

		inline static void DrawUint16Indexed(uint32_t count)
		{
			s_rendererAPI->DrawUint16Indexed(count);
		}

		inline static void DrawUint32Indexed(const Ptr<VertexArray>& vertexArray)
		{
			s_rendererAPI->DrawUint32Indexed(vertexArray);
		}

		inline static void DrawUint32Indexed(uint32_t count)
		{
			s_rendererAPI->DrawUint32Indexed(count);
		}

		inline static void DrawPatch(uint32_t count)
		{
			s_rendererAPI->DrawPatch(count);
		}
	private:
		static RendererAPI* s_rendererAPI;
	};
}
