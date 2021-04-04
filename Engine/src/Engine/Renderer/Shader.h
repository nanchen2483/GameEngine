#pragma once

#include <string>

namespace Engine
{
	class Shader {
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ptr<Shader> Create(const std::string& filePath);
		static Ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

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