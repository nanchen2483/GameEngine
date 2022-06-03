#include "enginepch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Engine
{
	RendererAPI* RendererCommand::s_rendererAPI = new OpenGLRendererAPI;
}
