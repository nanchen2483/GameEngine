#include "enginepch.h"
#include "ShaderFactory.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Ptr<IShader> ShaderFactory::Create(const std::string& filePath)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLShader>(filePath);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IShader> ShaderFactory::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
