#pragma once
#include "IShader.h"

namespace Engine
{
	class ShaderFactory
	{
	private:
		static Ptr<IShader> Create(const std::string& filePath);
		static Ptr<IShader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		friend class ShaderLibrary;
	};
}
