#pragma once
#include "Engine/Library/ILibrary.h"
#include "Shader.h"

namespace Engine
{
	class ShaderLibrary : public ILibrary<Shader>
	{
	public:
		virtual Ptr<Shader> Load(const std::string filePath) override;
		virtual Ptr<Shader> Get(const std::string& filePath) const override;
		virtual void Add(const Ptr<Shader> data) override;

		static ShaderLibrary* GetInstance();
	protected:
		virtual bool Exists(const std::string& filePath) const override;
	private:
		ShaderLibrary() = default;

		std::unordered_map<std::string, Ptr<Shader>> m_shaders;
		static ShaderLibrary* s_instance;

	};
}
