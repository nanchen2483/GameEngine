#pragma once

#include "IModel.h"

namespace Engine
{
	class ModelFactory : public IModel
	{
	public:
		static Ptr<IModel> Create(std::string path);
	};
}
