#pragma once
#include "Engine/Core/Base.h"

#include <string>
#include <unordered_map>

namespace Engine
{
	template<class T>
	class ILibrary
	{
	public:
		virtual Ptr<T> Load(const std::filesystem::path& filePath) = 0;
	protected:
		virtual Ptr<T> Get(const std::string& name) const = 0;
		virtual void Add(const Ptr<T> data) = 0;
		virtual bool Exists(const std::string& name) const = 0;
	};
}
