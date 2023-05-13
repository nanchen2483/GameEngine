#pragma once
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"
#include "Engine/Core/Enum/PolygonMode.h"
#include "Engine/Renderer/Texture/Texture.h"

namespace Engine
{
	class OpenGLUtil
	{
	public:
		static int32_t ToGL(DepthFunc func);
		static int32_t ToGL(FaceCulling face);
		static int32_t ToGL(TextureAccessType access);
		static TextureFormat ToGL(TextureFormatType type);
		static int32_t ToGL(PolygonMode mode);
		static PolygonMode FromGLPolygonMode(int mode);
	};
}

