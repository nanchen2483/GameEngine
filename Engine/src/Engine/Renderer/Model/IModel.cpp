#include "enginepch.h"
#include "IModel.h"

#include "Engine/Util/Assimp/AssimpModel.h"

namespace Engine
{
	Ptr<IModel> IModel::Create(std::string path)
	{
		return CreatePtr<AssimpModel>(path);
	}

	Ptr<IModel> IModel::Create(std::string path, bool gamma)
	{
		return CreatePtr<AssimpModel>(path, gamma);
	}
	
	Ptr<IModel> IModel::Create(std::string path, bool gamma, Ptr<float> progression)
	{
		return CreatePtr<AssimpModel>(path, gamma, progression);
	}
}
