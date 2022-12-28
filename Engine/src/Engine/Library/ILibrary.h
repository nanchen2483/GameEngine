#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/System/Uid/Uid.h"

#include <filesystem>

namespace Engine
{
	template<class T>
	class ILibrary
	{
	public:
		virtual Ptr<T> Load(const std::filesystem::path& filePath) = 0;
	protected:
		virtual Ptr<T> Get(const Uid& key) const = 0;
		virtual void Add(const Ptr<T> data) = 0;
		virtual bool Exists(const Uid& key) const = 0;
	};
}
