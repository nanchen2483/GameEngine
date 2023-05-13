#pragma once
#include "ILibrary.h"
#include "Engine/Renderer/Model/Model.h"

namespace Engine
{
	class ENGINE_API ModelLibrary : public ILibrary<Model>
	{
	public:
		static Ptr<Model> Load(const std::filesystem::path& filePath);
	private:
		ModelLibrary() = default;
		virtual Ptr<Model> Get(const Uid& key) const override;
		virtual void Add(const Ptr<Model> data) override;
		virtual bool Exists(const Uid& key) const override;
		static ModelLibrary& GetInstance();

		std::unordered_map<uint64_t, Ptr<Model>> m_models;
	};
}
