#include "enginepch.h"
#include "RendererCommand.h"

#include "Engine/Factory/GraphicsFactory.h"

namespace Engine
{
	void RendererCommand::Init()
	{
		GraphicsFactory::GetAPIInstance().Init();
	}

	void RendererCommand::CullFace(FaceCulling face)
	{
		GraphicsFactory::GetAPIInstance().CullFace(face);
	}

	std::pair<uint32_t, uint32_t> RendererCommand::GetViewport()
	{
		return GraphicsFactory::GetAPIInstance().GetViewport();
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GraphicsFactory::GetAPIInstance().SetViewport(x, y, width, height);
	}

	void RendererCommand::SetDepthFunc(DepthFunc func)
	{
		GraphicsFactory::GetAPIInstance().SetDepthFunc(func);
	}

	PolygonMode RendererCommand::GetPolygonMode()
	{
		return GraphicsFactory::GetAPIInstance().GetPolygonMode();
	}

	void RendererCommand::SetPolygonMode(PolygonMode mode)
	{
		GraphicsFactory::GetAPIInstance().SetPolygonMode(mode);
	}

	uint32_t RendererCommand::GetFramebuffer()
	{
		return GraphicsFactory::GetAPIInstance().GetFramebuffer();
	}

	void RendererCommand::SetFramebuffer(uint32_t framebuffer)
	{
		GraphicsFactory::GetAPIInstance().SetFramebuffer(framebuffer);
	}

	void RendererCommand::SetClearColor(const glm::vec4& color)
	{
		GraphicsFactory::GetAPIInstance().SetClearColor(color);
	}

	void RendererCommand::Clear()
	{
		GraphicsFactory::GetAPIInstance().Clear();
	}

	void RendererCommand::DrawArrays(uint32_t count)
	{
		GraphicsFactory::GetAPIInstance().DrawArrays(count);
	}

	void RendererCommand::DrawArraysStrip(uint32_t count)
	{
		GraphicsFactory::GetAPIInstance().DrawArraysStrip(count);
	}

	void RendererCommand::DrawUint8Indexed(uint32_t count)
	{
		GraphicsFactory::GetAPIInstance().DrawUint8Indexed(count);
	}

	void RendererCommand::DrawUint16Indexed(uint32_t count)
	{
		GraphicsFactory::GetAPIInstance().DrawUint16Indexed(count);
	}

	void RendererCommand::DrawUint32Indexed(const Ptr<IVertexArray>& vertexArray)
	{
		GraphicsFactory::GetAPIInstance().DrawUint32Indexed(vertexArray);
	}

	void RendererCommand::DrawUint32Indexed(uint32_t count)
	{
		GraphicsFactory::GetAPIInstance().DrawUint32Indexed(count);
	}

	void RendererCommand::DrawPatch(uint32_t count)
	{
		GraphicsFactory::GetAPIInstance().DrawPatch(count);
	}

	void RendererCommand::Compute(uint32_t numOfX, uint32_t numOfY, uint32_t numOfZ)
	{
		GraphicsFactory::GetAPIInstance().Compute(numOfX, numOfY, numOfZ);
	}
}
