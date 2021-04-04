#include "enginepch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment")
		{
			return GL_FRAGMENT_SHADER;
		}

		ENGINE_CORE_ASSERT(false, "Unknow shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::string source = ReadFile(filePath);
		auto shaderSource = PreProcess(source);
		Compile(shaderSource);

		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto nameSize = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_shaderName = filePath.substr(lastSlash, nameSize);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_shaderName(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_rendererId);
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				ENGINE_CORE_ERROR("Could not read from file '{0}'", path);
			}
		}
		else
		{
			ENGINE_CORE_ERROR("Could not open file '{0}'", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSource;

		const char* typeToken = "#type";
		size_t tokenLength = strlen(typeToken);
		size_t tokenPos = source.find(typeToken, 0);
		while (tokenPos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", tokenPos);
			ENGINE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = tokenPos + tokenLength + 1;
			std::string shaderType = source.substr(begin, eol - begin);
			ENGINE_CORE_ASSERT(ShaderTypeFromString(shaderType), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			ENGINE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			tokenPos = source.find(typeToken, nextLinePos);

			shaderSource[ShaderTypeFromString(shaderType)] = (tokenPos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, tokenPos - nextLinePos);
		}

		return shaderSource;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string> shaderSources)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		GLuint program = glCreateProgram();
		ENGINE_CORE_ASSERT(shaderSources.size() <= 5, "Only supprt max 5 shaders!");
		std::array<GLuint, 5> shaders;
		int shaderIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum shaderType = kv.first;
			const std::string& shaderSource = kv.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(shaderType);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* source = shaderSource.c_str();
			glShaderSource(shader, 1, &source, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.
				ENGINE_CORE_ERROR("{0}", infoLog.data());
				ENGINE_CORE_ASSERT(false, "Vertex shader compilation failure!");

				// In this simple program, we'll just leave
				return;
			}
			
			// Attach our shaders to our program
			glAttachShader(program, shader);
			shaders[shaderIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto shader : shaders)
			{
				glDeleteShader(shader);
			}

			// Use the infoLog as you see fit.
			ENGINE_CORE_ERROR("{0}", infoLog.data());
			ENGINE_CORE_ASSERT(false, "Shader link failure!");

			// In this simple program, we'll just leave
			return;
		}

		for (auto shader : shaders)
		{
			// Always detach shaders after a successful link.
			glDetachShader(program, shader);
		}

		m_rendererId = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_rendererId);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_rendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

