#include "enginepch.h"
#include "ModelLibrary.h"

namespace Engine
{
	Ptr<Model> ModelLibrary::Load(const std::filesystem::path& filePath)
	{
		ModelLibrary& instance = GetInstance();

		const Uid uid = Uid::NewUid(filePath.string());
		if (instance.Exists(uid))
		{
			return instance.Get(uid);
		}

		Ptr<Model> model = Model::Create(filePath.string());
		instance.Add(model);

		return model;
	}

	void ModelLibrary::Add(const Ptr<Model> data)
	{
		const Uid& uid = data->GetUid();
		ENGINE_CORE_ASSERT(!Exists(uid), "Model already exists!");
		m_models.emplace(uid, data);
	}

	Ptr<Model> ModelLibrary::Get(const Uid& key) const
	{
		ENGINE_CORE_ASSERT(Exists(key), "Model not found!");
		return m_models.at(key);
	}
	
	bool ModelLibrary::Exists(const Uid& key) const
	{
		return m_models.find(key) != m_models.end();
	}

	ModelLibrary& ModelLibrary::GetInstance()
	{
		static ModelLibrary instance;
		return instance;
	}
}
