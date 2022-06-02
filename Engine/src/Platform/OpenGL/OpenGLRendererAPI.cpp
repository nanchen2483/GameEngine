#include "enginepch.h"
#include "OpenGLRendererAPI.h"

#include "Platform/Util/OpenGLUtil.h"

#include <glad/glad.h>

namespace Engine
{
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::CullFace(FaceCulling face)
	{
		glCullFace(OpenGLUtil::ToGL(face));
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetDepthFunc(DepthFunc func)
	{
		glDepthFunc(OpenGLUtil::ToGL(func));
	}

	void OpenGLRendererAPI::SetPolygonMode(PolygonMode mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, OpenGLUtil::ToGL(mode));
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawUint8Indexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, nullptr);
	}

	void OpenGLRendererAPI::DrawUint16Indexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
	}

	void OpenGLRendererAPI::DrawUint32Indexed(const Ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetNumOfIndices(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawUint32Indexed(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawPatch(uint32_t count)
	{
		glDrawArrays(GL_PATCHES, 0, count);
	}
}
