#include "enginepch.h"
#include "Model.h"

#include "Engine/Util/Assimp/AssimpModel.h"

namespace Engine
{
	Ptr<Model> Model::Create(std::string path, bool gamma)
	{
		return CreatePtr<AssimpModel>(path, gamma);
	}

	Ptr<Model> Model::Create(std::string path, bool gamma, uint32_t entityId)
	{
		return CreatePtr<AssimpModel>(path, gamma, entityId);
	}
	
	Ptr<Model> Model::Create(std::string path, bool gamma, uint32_t entityId, Ptr<float> progression)
	{
		return CreatePtr<AssimpModel>(path, gamma, entityId, progression);
	}
}
