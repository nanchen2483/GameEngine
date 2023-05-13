#pragma once
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"
#include "Graphics/GraphicsAPI.h"

namespace Engine
{
	class ENGINE_API RendererCommand
	{
	public:
		static void Init();
		static void CullFace(FaceCulling face);
		static void  SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetDepthFunc(DepthFunc func);
		static PolygonMode GetPolygonMode();
		static void SetPolygonMode(PolygonMode mode);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawUint8Indexed(uint32_t count);
		static void DrawUint16Indexed(uint32_t count);
		static void DrawUint32Indexed(const Ptr<VertexArray>& vertexArray);
		static void DrawUint32Indexed(uint32_t count);
		static void DrawPatch(uint32_t count);
		static void Compute(uint32_t numOfX, uint32_t numOfY, uint32_t numOfZ);
	private:
		RendererCommand();
		static RendererCommand& GetInstance();

		Uniq<GraphicsAPI> m_graphicsAPI;
	};
}
