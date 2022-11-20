#include "enginepch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
	Uniq<RendererAPI> RendererCommand::s_rendererAPI = CreateUniq<OpenGLRendererAPI>();
}
