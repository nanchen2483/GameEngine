#include "enginepch.h"
#include "ModelFactory.h"

#include "Model.h"

namespace Engine
{
	Ptr<IModel> ModelFactory::Create(std::string path)
	{
		return CreatePtr<Model>(path);
	}

	Ptr<IModel> ModelFactory::Create(std::string path, bool gamma)
	{
		return CreatePtr<Model>(path, gamma);
	}

	Ptr<IModel> ModelFactory::Create(std::string path, bool gamma, uint32_t entityId)
	{
		return CreatePtr<Model>(path, gamma, entityId);
	}
}
