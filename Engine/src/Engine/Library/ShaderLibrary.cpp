#include "enginepch.h"
#include "ShaderLibrary.h"

namespace Engine
{
	Ptr<Shader> ShaderLibrary::Load(const std::filesystem::path& filePath)
	{
		ShaderLibrary& instance = GetInstance();

		Uid uid = Uid::NewUid(filePath.string());
		if (instance.Exists(uid))
		{
			return instance.Get(uid);
		}

		Ptr<Shader> shader = Shader::Create(filePath.string());
		instance.Add(shader);

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
		m_shaders.emplace(uid, data);
	}

	bool ShaderLibrary::Exists(const Uid& uid) const
	{
		return m_shaders.find(uid) != m_shaders.end();
	}

	ShaderLibrary& ShaderLibrary::GetInstance()
	{
		static ShaderLibrary instance;
		return instance;
	}
}
