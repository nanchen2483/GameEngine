#include "enginepch.h"
#include "OpenGLGraphicsAPI.h"

#include "Platform/Util/OpenGLUtil.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLGraphicsAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLGraphicsAPI::CullFace(FaceCulling face)
	{
		glCullFace(OpenGLUtil::ToGL(face));
	}

	std::pair<uint32_t, uint32_t> OpenGLGraphicsAPI::GetViewport()
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		return { (uint32_t)viewport[2], (uint32_t)viewport[3] };
	}

	void OpenGLGraphicsAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	uint32_t OpenGLGraphicsAPI::GetFramebuffer()
	{
		int framebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);

		return framebuffer;
	}

	void OpenGLGraphicsAPI::SetFramebuffer(uint32_t framebuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	}

	void OpenGLGraphicsAPI::SetDepthFunc(DepthFunc func)
	{
		glDepthFunc(OpenGLUtil::ToGL(func));
	}

	PolygonMode OpenGLGraphicsAPI::GetPolygonMode()
	{
		int polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);

		return OpenGLUtil::FromGLPolygonMode(polygonMode[0]);
	}

	void OpenGLGraphicsAPI::SetPolygonMode(PolygonMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, OpenGLUtil::ToGL(mode));
	}

	void OpenGLGraphicsAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLGraphicsAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLGraphicsAPI::DrawUint8Indexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, nullptr);
	}

	void OpenGLGraphicsAPI::DrawUint16Indexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
	}

	void OpenGLGraphicsAPI::DrawUint32Indexed(const Ptr<IVertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLGraphicsAPI::DrawUint32Indexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLGraphicsAPI::DrawPatch(uint32_t count)
	{
		glDrawArrays(GL_PATCHES, 0, count);
	}
	
	void OpenGLGraphicsAPI::Compute(uint32_t numOfX, uint32_t numOfY, uint32_t numOfZ)
	{
		glDispatchCompute(numOfX, numOfY, numOfZ);
	}
}
