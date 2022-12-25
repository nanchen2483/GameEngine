#include "enginepch.h"
#include "ShaderLibrary.h"

namespace Engine
{
	ShaderLibrary* ShaderLibrary::s_instance = nullptr;

	Ptr<Shader> ShaderLibrary::Load(const std::string filePath)
	{
		if (Exists(filePath))
		{
			return Get(filePath);
		}

		Ptr<Shader> shader = Shader::Create(filePath);
		Add(shader);

		return shader;
	}

	Ptr<Shader> ShaderLibrary::Get(const std::string& filePath) const
	{
		ENGINE_CORE_ASSERT(Exists(filePath), "Shader not found!");
		return m_shaders.at(filePath);
	}

	void ShaderLibrary::Add(const Ptr<Shader> data)
	{
		std::string shaderName = data->GetName();
		ENGINE_CORE_ASSERT(!Exists(shaderName), "Shader already exists!");
		m_shaders.insert({ shaderName, data });
	}

	bool ShaderLibrary::Exists(const std::string& filePath) const
	{
		return m_shaders.find(filePath) != m_shaders.end();
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
