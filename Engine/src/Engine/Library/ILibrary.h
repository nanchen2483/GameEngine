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
		virtual Ptr<T> Load(const std::string filePath) = 0;
		virtual Ptr<T> Get(const std::string& filePath) const = 0;
		virtual void Add(const Ptr<T> data) = 0;
	protected:
		virtual bool Exists(const std::string& filePath) const = 0;
	};
}
