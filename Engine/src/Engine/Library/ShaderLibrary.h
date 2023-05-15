#pragma once
#include "ILibrary.h"
#include "Engine/Renderer/Shader/IShader.h"

#include <filesystem>

namespace Engine
{
	class ENGINE_API ShaderLibrary : public ILibrary<IShader>
	{
	public:
		static Ptr<IShader> Load(const std::filesystem::path& filePath);
	private:
		ShaderLibrary() = default;
		virtual Ptr<IShader> Get(const Uid& key) const override;
		virtual void Add(const Ptr<IShader> data) override;
		virtual bool Exists(const Uid& key) const override;
		static ShaderLibrary& GetInstance();

		std::unordered_map<uint64_t, Ptr<IShader>> m_shaders;
	};
}
