#include "enginepch.h"
#include "Model.h"

#include "Platform/Assimp/AssimpModel.h"

namespace Engine
{
	Ptr<Model> Model::Create(std::string path)
	{
		return CreatePtr<AssimpModel>(path);
	}

	Ptr<Model> Model::Create(std::string path, bool gamma)
	{
		return CreatePtr<AssimpModel>(path, gamma);
	}

	Ptr<Model> Model::Create(std::string path, bool gamma, uint32_t entityId)
	{
		return CreatePtr<AssimpModel>(path, gamma, entityId);
	}
}

