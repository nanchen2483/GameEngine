#include "enginepch.h"
#include "ModelFactory.h"

#include "Model.h"

namespace Engine
{
	Ptr<IModel> ModelFactory::Create(std::string path)
	{
		return CreatePtr<Model>(path);
	}
}
