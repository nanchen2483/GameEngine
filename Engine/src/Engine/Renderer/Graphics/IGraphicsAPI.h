#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"
#include "Engine/Core/Enum/PolygonMode.h"
#include "Engine/Renderer/VertexArray/VertexArray.h"

#include <glm/glm.hpp>

namespace Engine
{
	class IGraphicsAPI
	{
	public:
		virtual ~IGraphicsAPI() = default;
		virtual void Init() = 0;
		virtual void CullFace(FaceCulling face) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetDepthFunc(DepthFunc func) = 0;
		virtual PolygonMode GetPolygonMode() = 0;
		virtual void SetPolygonMode(PolygonMode mode) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawUint8Indexed(uint32_t count) = 0;
		virtual void DrawUint16Indexed(uint32_t count) = 0;
		virtual void DrawUint32Indexed(uint32_t count) = 0;
		virtual void DrawUint32Indexed(const Ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawPatch(uint32_t count) = 0;
		virtual void Compute(uint32_t numOfX, uint32_t numOfY, uint32_t numOfZ) = 0;

		static Uniq<IGraphicsAPI> Create();
	};
}
