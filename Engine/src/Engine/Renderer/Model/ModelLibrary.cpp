#include "enginepch.h"
#include "ModelLibrary.h"

namespace Engine
{
	ModelLibrary* ModelLibrary::s_instance = nullptr;

	Ptr<Model> ModelLibrary::Load(const std::filesystem::path& filePath)
	{
		return Load(filePath, -1);
	}

	Ptr<Model> ModelLibrary::Load(const std::filesystem::path& filePath, uint32_t entityId)
	{
		const Uid uid = Uid::NewUid(filePath.string());
		if (Exists(uid))
		{
			return Get(uid);
		}

		Ptr<Model> model = Model::Create(filePath.string(), false, entityId);
		Add(model);

		return model;
	}

	void ModelLibrary::Add(const Ptr<Model> data)
	{
		const Uid& uid = data->GetUid();
		ENGINE_CORE_ASSERT(!Exists(uid), "Model already exists!");
		m_models.insert({ uid, data });
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

	ModelLibrary* ModelLibrary::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new ModelLibrary();
		}

		return s_instance;
	}
}
