#include "enginepch.h"
#include "RendererCommand.h"

#include "Engine/Renderer/Graphics/GraphicsFactory.h"

namespace Engine
{
	void RendererCommand::Init()
	{
		GetInstance().m_graphicsAPI->Init();
	}

	void RendererCommand::CullFace(FaceCulling face)
	{
		GetInstance().m_graphicsAPI->CullFace(face);
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		GetInstance().m_graphicsAPI->SetViewport(x, y, width, height);
	}

	void RendererCommand::SetDepthFunc(DepthFunc func)
	{
		GetInstance().m_graphicsAPI->SetDepthFunc(func);
	}

	PolygonMode RendererCommand::GetPolygonMode()
	{
		return GetInstance().m_graphicsAPI->GetPolygonMode();
	}

	void RendererCommand::SetPolygonMode(PolygonMode mode)
	{
		GetInstance().m_graphicsAPI->SetPolygonMode(mode);
	}

	void RendererCommand::SetClearColor(const glm::vec4& color)
	{
		GetInstance().m_graphicsAPI->SetClearColor(color);
	}

	void RendererCommand::Clear()
	{
		GetInstance().m_graphicsAPI->Clear();
	}

	void RendererCommand::DrawUint8Indexed(uint32_t count)
	{
		GetInstance().m_graphicsAPI->DrawUint8Indexed(count);
	}

	void RendererCommand::DrawUint16Indexed(uint32_t count)
	{
		GetInstance().m_graphicsAPI->DrawUint16Indexed(count);
	}

	void RendererCommand::DrawUint32Indexed(const Ptr<IVertexArray>& vertexArray)
	{
		GetInstance().m_graphicsAPI->DrawUint32Indexed(vertexArray);
	}

	void RendererCommand::DrawUint32Indexed(uint32_t count)
	{
		GetInstance().m_graphicsAPI->DrawUint32Indexed(count);
	}

	void RendererCommand::DrawPatch(uint32_t count)
	{
		GetInstance().m_graphicsAPI->DrawPatch(count);
	}

	void RendererCommand::Compute(uint32_t numOfX, uint32_t numOfY, uint32_t numOfZ)
	{
		GetInstance().m_graphicsAPI->Compute(numOfX, numOfY, numOfZ);
	}

	RendererCommand::RendererCommand()
		: m_graphicsAPI(GraphicsFactory::CreateAPI())
	{
	}

	RendererCommand& RendererCommand::GetInstance()
	{
		static RendererCommand instance;
		return instance;
	}
}
