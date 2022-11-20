#include "enginepch.h"
#include "ShaderLibrary.h"

namespace Engine
{
	void ShaderLibrary::Add(const std::string& name, const Ptr<Shader>& shader)
	{
		ENGINE_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ptr<Shader>& shader)
	{
		std::string shaderName = shader->GetName();
		Add(shaderName, shader);
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		Ptr<Shader> shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		Ptr<Shader> shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		Ptr<Shader> shader = Shader::Create(name, vertexSrc, fragmentSrc);
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
