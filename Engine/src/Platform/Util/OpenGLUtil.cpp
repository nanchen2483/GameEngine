#include "OpenGLUtil.h"

#include "Engine/Core/Base.h"

#include <glad/glad.h>

namespace Engine
{
	int32_t OpenGLUtil::ToGL(DepthFunc func)
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

	int32_t OpenGLUtil::ToGL(FaceCulling face)
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

	int32_t OpenGLUtil::ToGL(TextureAccessType access)
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

	TextureFormat OpenGLUtil::ToGL(TextureFormatType type)
	{
		TextureFormat format;
		switch (type)
		{
		case TextureFormatType::RED:
			format.internalFormat = GL_RED;
			format.dataFormat = GL_RED;
			break;
		case TextureFormatType::R16:
			format.internalFormat = GL_R16F;
			format.dataFormat = GL_RED;
			break;
		case TextureFormatType::R32:
			format.internalFormat = GL_R32F;
			format.dataFormat = GL_RED;
			break;
		case TextureFormatType::RG8:
			format.internalFormat = GL_RG8;
			format.dataFormat = GL_RG;
			break;
		case TextureFormatType::RG16:
			format.internalFormat = GL_RG16F;
			format.dataFormat = GL_RG;
			break;
		case TextureFormatType::RG32:
			format.internalFormat = GL_RG32F;
			format.dataFormat = GL_RG;
			break;
		case TextureFormatType::RGB8:
			format.internalFormat = GL_RGB8;
			format.dataFormat = GL_RGB;
			break;
		case TextureFormatType::RGB16:
			format.internalFormat = GL_RGB16F;
			format.dataFormat = GL_RGB;
			break;
		case TextureFormatType::RGB32:
			format.internalFormat = GL_RGB32F;
			format.dataFormat = GL_RGB;
			break;
		case TextureFormatType::RGBA8:
			format.internalFormat = GL_RGBA8;
			format.dataFormat = GL_RGBA;
			break;
		case TextureFormatType::RGBA16:
			format.internalFormat = GL_RGBA16F;
			format.dataFormat = GL_RGBA;
			break;
		case TextureFormatType::RGBA32:
			format.internalFormat = GL_RGBA32F;
			format.dataFormat = GL_RGBA;
			break;
		default:
			ENGINE_CORE_ASSERT(false, "Invalid internal format type");
		}

		return format;
	}

	int32_t OpenGLUtil::ToGL(PolygonMode mode)
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

	PolygonMode OpenGLUtil::FromGLPolygonMode(int mode)
	{
		switch (mode)
		{
		case GL_POINT:
			return PolygonMode::POINT;
		case GL_LINE:
			return PolygonMode::LINE;
		case GL_FILL:
			return PolygonMode::FILL;
		default:
			ENGINE_CORE_ASSERT(false, "Invalid polygonMode type");
			return (PolygonMode)-1;
		}
	}
}
