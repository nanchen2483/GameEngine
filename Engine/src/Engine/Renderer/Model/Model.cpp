#include "enginepch.h"
#include "Model.h"

#include "Platform/Assimp/AssimpModel.h"

namespace Engine
{
	Ptr<Model> Model::Create(std::string path, bool gamma)
	{
		return CreatePtr<AssimpModel>(path, gamma);
	}

	Ptr<Model> Model::Create(std::string path, bool gamma, uint32_t entityId, Ptr<TextureMap> textureMap)
	{
		return CreatePtr<AssimpModel>(path, gamma, entityId, textureMap);
	}
	
	Ptr<Model> Model::Create(std::string path, bool gamma, uint32_t entityId, Ptr<TextureMap> textureMap, Ptr<float> progression)
	{
		return CreatePtr<AssimpModel>(path, gamma, entityId, textureMap, progression);
	}
}

