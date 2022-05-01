#pragma once
#include <glad/glad.h>

#include "Engine/Core/Base.h"
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"

namespace Engine
{
	class OpenGLUtil
	{
	public:
		static inline int ToGL(DepthFunc func)
		{
			switch (func)
			{
			case DepthFunc::NEVER:
				return GL_NEVER;
			case DepthFunc::LESS:
				return GL_LESS;
			case DepthFunc::LEQUAL:
				return GL_LEQUAL;
			case DepthFunc::EQUAL:
				return GL_EQUAL;
			case DepthFunc::GREATER:
				return GL_GREATER;
			case DepthFunc::NOTEQUAL:
				return GL_NOTEQUAL;
			case DepthFunc::GEQUAL:
				return GL_GEQUAL;
			case DepthFunc::ALWAYS:
				return GL_ALWAYS;
			case DepthFunc::UNKNOWN:
			default:
				ENGINE_CORE_ASSERT(false, "Invalid depth-comparison function {0}", func);
				break;
			}

			return -1;
		}

		static inline int ToGL(FaceCulling face)
		{
			switch (face)
			{
			case FaceCulling::FRONT:
				return GL_FRONT;
			case FaceCulling::BACK:
				return GL_BACK;
			case FaceCulling::UNKNOWN:
			default:
				ENGINE_CORE_ASSERT(false, "Invalid depth-comparison function {0}", face);
				break;
			}

			return -1;
		}
	};
}

