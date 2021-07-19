#include "enginepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	/// <summary>
	/// Shader
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	Ptr<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreatePtr<OpenGLShader>(filePath);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
	
	Ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return CreatePtr<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	/// <summary>
	/// Shader library
	/// </summary>
	/// <param name="name"></param>
	/// <param name="shader"></param>

	void ShaderLibrary::Add(const std::string& name, const Ptr<Shader>& shader)
	{
		ENGINE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ptr<Shader>& shader)
	{
		auto shaderName = shader->GetName();
		ShaderLibrary::Add(shaderName, shader);
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}
	
	Ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		auto shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(name, shader);
		return shader;
	}

	Ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ENGINE_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_shaders.find(name) != m_shaders.end();
	}
}
