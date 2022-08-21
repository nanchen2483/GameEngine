#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Enum/DepthFunc.h"
#include "Engine/Core/Enum/FaceCulling.h"
#include "Engine/Core/Enum/PolygonMode.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <glad/glad.h>

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
			default:
				ENGINE_CORE_ASSERT(false, "Invalid depth-comparison function");
				return -1;
			}
		}

		static inline int ToGL(FaceCulling face)
		{
			switch (face)
			{
			case FaceCulling::FRONT:
				return GL_FRONT;
			case FaceCulling::BACK:
				return GL_BACK;
			default:
				ENGINE_CORE_ASSERT(false, "Invalid faceCulling type");
				return -1;
			}
		}

		static inline int ToGL(PolygonMode mode)
		{
			switch (mode)
			{
			case PolygonMode::POINT:
				return GL_POINT;
			case PolygonMode::LINE:
				return GL_LINE;
			case PolygonMode::FILL:
				return GL_FILL;
			default:
				ENGINE_CORE_ASSERT(false, "Invalid polygonMode type");
				return -1;
			}
		}

		static inline int ToGL(TextureAccessType access)
		{
			switch (access)
			{
			case TextureAccessType::ReadOnly:
				return GL_READ_ONLY;
			case TextureAccessType::WriteOnly:
				return GL_WRITE_ONLY;
			case TextureAccessType::ReadWrite:
				return GL_READ_WRITE;
			default:
				ENGINE_CORE_ASSERT(false, "Invalid TextureAccess type");
				return -1;
			}
		}

		static inline TextureFormat ToGL(TextureFormatType type)
		{
			TextureFormat format;
			switch (type)
			{
			case Engine::TextureFormatType::RED:
				format.internalFormat = GL_RED;
				format.dataFormat = GL_RED;
				break;
			case Engine::TextureFormatType::R16:
				format.internalFormat = GL_R16F;
				format.dataFormat = GL_RED;
				break;
			case Engine::TextureFormatType::R32:
				format.internalFormat = GL_R32F;
				format.dataFormat = GL_RED;
				break;
			case Engine::TextureFormatType::RG8:
				format.internalFormat = GL_RG8;
				format.dataFormat = GL_RG;
				break;
			case Engine::TextureFormatType::RG16:
				format.internalFormat = GL_RG16F;
				format.dataFormat = GL_RG;
				break;
			case Engine::TextureFormatType::RG32:
				format.internalFormat = GL_RG32F;
				format.dataFormat = GL_RG;
				break;
			case Engine::TextureFormatType::RGB8:
				format.internalFormat = GL_RGB8;
				format.dataFormat = GL_RGB;
				break;
			case Engine::TextureFormatType::RGB16:
				format.internalFormat = GL_RGB16F;
				format.dataFormat = GL_RGB;
				break;
			case Engine::TextureFormatType::RGB32:
				format.internalFormat = GL_RGB32F;
				format.dataFormat = GL_RGB;
				break;
			case Engine::TextureFormatType::RGBA8:
				format.internalFormat = GL_RGBA8;
				format.dataFormat = GL_RGBA;
				break;
			case Engine::TextureFormatType::RGBA16:
				format.internalFormat = GL_RGBA16F;
				format.dataFormat = GL_RGBA;
				break;
			case Engine::TextureFormatType::RGBA32:
				format.internalFormat = GL_RGBA32F;
				format.dataFormat = GL_RGBA;
				break;
			default:
				ENGINE_CORE_ASSERT(false, "Invalid internal format type");
			}

			return format;
		}
	};
}

