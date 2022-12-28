#include "enginepch.h"
#include "ShaderLibrary.h"

namespace Engine
{
	ShaderLibrary* ShaderLibrary::s_instance = nullptr;

	Ptr<Shader> ShaderLibrary::Load(const std::filesystem::path& filePath)
	{
		Uid uid = Uid::NewUid(filePath.stem().string());
		if (Exists(uid))
		{
			return Get(uid);
		}

		Ptr<Shader> shader = Shader::Create(filePath.string());
		Add(shader);

		return shader;
	}

	Ptr<Shader> ShaderLibrary::Get(const Uid& key) const
	{
		ENGINE_CORE_ASSERT(Exists(key), "Shader not found!");
		return m_shaders.at(key);
	}

	void ShaderLibrary::Add(const Ptr<Shader> data)
	{
		const Uid& uid = data->GetUid();
		ENGINE_CORE_ASSERT(!Exists(uid), "Shader already exists!");
		m_shaders.insert({ uid, data });
	}

	bool ShaderLibrary::Exists(const Uid& uid) const
	{
		return m_shaders.find(uid) != m_shaders.end();
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
