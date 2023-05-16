#pragma once
#include "Engine/Core/Enum/CursorMode.h"
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"
#include "Engine/Core/Enum/PolygonMode.h"
#include "Engine/Renderer/Texture/TextureFormat.h"
#include "Engine/Renderer/Texture/TextureType.h"

namespace Engine
{
	class OpenGLUtil
	{
	public:
		static int32_t ToGL(CursorMode mode);
		static CursorMode FromGLCursorMode(int mode);
		static int32_t ToGL(DepthFunc func);
		static int32_t ToGL(FaceCulling face);
		static int32_t ToGL(TextureAccessType access);
		static TextureFormat ToGL(TextureFormatType type);
		static int32_t ToGL(PolygonMode mode);
		static PolygonMode FromGLPolygonMode(int mode);
	};
}

