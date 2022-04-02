#pragma once

#include "IModel.h"

namespace Engine
{
	class ModelFactory : public IModel
	{
	public:
		static Ptr<IModel> Create(std::string path);
		static Ptr<IModel> Create(std::string path, bool gamma);
		static Ptr<IModel> Create(std::string path, bool gamma, uint32_t entityId);
	};
}
