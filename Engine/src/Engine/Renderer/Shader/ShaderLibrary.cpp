#include "enginepch.h"
#include "ShaderLibrary.h"

namespace Engine
{
	ShaderLibrary* ShaderLibrary::s_instance = nullptr;

	void ShaderLibrary::Add(const Ptr<Shader>& shader)
	{
		std::string shaderName = shader->GetName();
		ENGINE_CORE_ASSERT(!Exists(shaderName), "Shader already exists!");
		m_shaders[shaderName] = shader;
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		if (Exists(filePath))
		{
			return Get(filePath);
		}

		Ptr<Shader> shader = Shader::Create(filePath);
		Add(shader);

		return shader;
	}

	Ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		if (Exists(name))
		{
			return Get(name);
		}

		Ptr<Shader> shader = Shader::Create(name, vertexSrc, fragmentSrc);
		Add(shader);

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

	ShaderLibrary* ShaderLibrary::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new ShaderLibrary();
		}

		return s_instance;
	}
}
