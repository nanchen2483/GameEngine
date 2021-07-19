#pragma once

#include <unordered_map>

#include "Shader.h"
#include "Engine/Core/Base.h"

namespace Engine
{
	class ShaderLibrary
	{
	public:
		void Add(const Ptr<Shader>& shader);
		void Add(const std::string& name, const Ptr<Shader>& shader);
		Ptr<Shader> Load(const std::string& filePath);
		Ptr<Shader> Load(const std::string& name, const std::string& filePath);
		Ptr<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		Ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ptr<Shader>> m_shaders;
	};
}
