#pragma once

#include <glm/glm.hpp>
#include "Vertex/VertexArray.h"
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"

namespace Engine
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;

		virtual void CullFace(FaceCulling face) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetDepthFunc(DepthFunc func) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawUint8Indexed(uint32_t count) = 0;
		virtual void DrawUint16Indexed(uint32_t count) = 0;
		virtual void DrawUint32Indexed(uint32_t count) = 0;
		virtual void DrawUint32Indexed(const Ptr<VertexArray>& vertexArray) = 0;
		virtual void DrawPatch(uint32_t count) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
}
