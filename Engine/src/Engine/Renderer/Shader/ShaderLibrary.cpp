#include "enginepch.h"
#include "ShaderLibrary.h"

namespace Engine
{
	ShaderLibrary* ShaderLibrary::s_instance = nullptr;

	Ptr<Shader> ShaderLibrary::Load(const std::filesystem::path& filePath)
	{
		const std::string name = filePath.stem().string();
		if (Exists(name))
		{
			return Get(name);
		}

		Ptr<Shader> shader = Shader::Create(filePath.string());
		Add(shader);

		return shader;
	}

	Ptr<Shader> ShaderLibrary::Get(const std::string& name) const
	{
		ENGINE_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_shaders.at(name);
	}

	void ShaderLibrary::Add(const Ptr<Shader> data)
	{
		std::string shaderName = data->GetName();
		ENGINE_CORE_ASSERT(!Exists(shaderName), "Shader already exists!");
		m_shaders.insert({ shaderName, data });
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
