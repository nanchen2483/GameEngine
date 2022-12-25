#pragma once
#include "Engine/Library/ILibrary.h"
#include "Shader.h"

#include <filesystem>

namespace Engine
{
	class ShaderLibrary : public ILibrary<Shader>
	{
	public:
		virtual Ptr<Shader> Load(const std::filesystem::path& filePath) override;

		static ShaderLibrary* GetInstance();
	private:
		ShaderLibrary() = default;
		virtual Ptr<Shader> Get(const std::string& name) const override;
		virtual void Add(const Ptr<Shader> data) override;
		virtual bool Exists(const std::string& name) const override;

		std::unordered_map<std::string, Ptr<Shader>> m_shaders;
		static ShaderLibrary* s_instance;
	};
}
