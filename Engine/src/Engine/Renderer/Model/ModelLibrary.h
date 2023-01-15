#pragma once
#include "Engine/Library/ILibrary.h"
#include "Model.h"

namespace Engine
{
	class ModelLibrary : public ILibrary<Model>
	{
	public:
		virtual Ptr<Model> Load(const std::filesystem::path& filePath) override;

		static ModelLibrary* GetInstance();
	private:
		ModelLibrary() = default;
		virtual Ptr<Model> Get(const Uid& key) const override;
		virtual void Add(const Ptr<Model> data) override;
		virtual bool Exists(const Uid& key) const override;

		std::unordered_map<uint64_t, Ptr<Model>> m_models;
		static ModelLibrary* s_instance;
	};
}
