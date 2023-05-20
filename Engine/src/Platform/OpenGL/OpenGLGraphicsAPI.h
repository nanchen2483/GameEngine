#pragma once
#include "Engine/Renderer/Graphics/GraphicsAPI/IGraphicsAPI.h"

namespace Engine
{
	class OpenGLGraphicsAPI : public IGraphicsAPI
	{
	public:
		virtual void Init() override;
		virtual void CullFace(FaceCulling face) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetDepthFunc(DepthFunc func) override;
		virtual PolygonMode GetPolygonMode() override;
		virtual void SetPolygonMode(PolygonMode mode) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawUint8Indexed(uint32_t count) override;
		virtual void DrawUint16Indexed(uint32_t count) override;
		virtual void DrawUint32Indexed(uint32_t count) override;
		virtual void DrawUint32Indexed(const Ptr<IVertexArray>& vertexArray) override;
		virtual void DrawPatch(uint32_t count) override;
		virtual void Compute(uint32_t numOfX, uint32_t numOfY, uint32_t numOfZ) override;
	};
}
