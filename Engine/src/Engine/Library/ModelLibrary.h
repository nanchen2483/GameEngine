#pragma once
#include "ILibrary.h"
#include "Engine/Renderer/Model/IModel.h"

namespace Engine
{
	class ENGINE_API ModelLibrary : public ILibrary<IModel>
	{
	public:
		static Ptr<IModel> Load(const std::filesystem::path& filePath);
	private:
		ModelLibrary() = default;
		virtual Ptr<IModel> Get(const Uid& key) const override;
		virtual void Add(const Ptr<IModel> data) override;
		virtual bool Exists(const Uid& key) const override;
		static ModelLibrary& GetInstance();

		std::unordered_map<uint64_t, Ptr<IModel>> m_models;
	};
}
