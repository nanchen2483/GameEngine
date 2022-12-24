#pragma once
#include "Engine/Core/Base.h"
#include "Shader.h"

#include <unordered_map>

namespace Engine
{
	class ShaderLibrary
	{
	public:
		void Add(const Ptr<Shader>& shader);

		Ptr<Shader> Load(const std::string& filePath);
		Ptr<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		Ptr<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;

		static ShaderLibrary* GetInstance();
	private:
		ShaderLibrary() = default;

		std::unordered_map<std::string, Ptr<Shader>> m_shaders;
		static ShaderLibrary* s_instance;
	};
}
