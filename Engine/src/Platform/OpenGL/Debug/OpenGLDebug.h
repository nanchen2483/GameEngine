#pragma once
#include <glad/glad.h>
#include <string>

namespace Engine
{
	class OpenGLDebug
	{
	public:
		static inline std::string GetErrorMessage() { return glErrorMessage; }
		static inline bool IsValid()
		{
			glErrorMessage = "";
			GLenum errorCode = glGetError();
			switch (errorCode)
			{
			case GL_NO_ERROR:
				return true;
			case GL_INVALID_ENUM:
				glErrorMessage = "Error occurred: GL_INVALID_ENUM";
				return false;
			case GL_INVALID_VALUE:
				glErrorMessage = "Error occurred: GL_INVALID_VALUE";
				return false;
			case GL_INVALID_OPERATION:
				glErrorMessage = "Error occurred: GL_INVALID_OPERATION";
				return false;
			case GL_STACK_OVERFLOW:
				glErrorMessage = "Error occurred: GL_STACK_OVERFLOW";
				return false;
			case GL_STACK_UNDERFLOW:
				glErrorMessage = "Error occurred: GL_STACK_UNDERFLOW";
				return false;
			case GL_OUT_OF_MEMORY:
				glErrorMessage = "Error occurred: GL_OUT_OF_MEMORY";
				return false;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				glErrorMessage = "Error occurred: GL_INVALID_FRAMEBUFFER_OPERATION";
				return false;
			case GL_CONTEXT_LOST:
				glErrorMessage = "Error occurred: GL_CONTEXT_LOST";
				return false;
			default:
				glErrorMessage = "Unknown errorCode: " + errorCode;
				return false;
				break;
			}
		}
	private:
		static inline std::string glErrorMessage;
	};
}
