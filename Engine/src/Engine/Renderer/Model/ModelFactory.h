#pragma once
#include "IModel.h"

namespace Engine
{
	class ModelFactory
	{
	private:
		static Ptr<IModel> Create(std::string path);
		static Ptr<IModel> Create(std::string path, bool gamma);
		static Ptr<IModel> Create(std::string path, bool gamma, Ptr<float> progression);

		friend class ModelLibrary;
	};
}
