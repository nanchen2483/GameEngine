#pragma once
#include "ILibrary.h"
#include "Engine/Renderer/Shader/Shader.h"

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
		virtual Ptr<Shader> Get(const Uid& key) const override;
		virtual void Add(const Ptr<Shader> data) override;
		virtual bool Exists(const Uid& key) const override;

		std::unordered_map<uint64_t, Ptr<Shader>> m_shaders;
		static ShaderLibrary* s_instance;
	};
}
