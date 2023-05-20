#pragma once
#include "Engine/Renderer/Graphics/IGraphicsAPI.h"
#include "Engine/Renderer/Graphics/IGraphicsContext.h"
#include "Engine/Renderer/Graphics/IGraphicsLibrary.h"
#include "Engine/Core/Window/WindowUserData.h"

namespace Engine
{
	class GraphicsFactory
	{
	public:
		static Uniq<IGraphicsAPI> CreateAPI();
		static Uniq<IGraphicsContext> CreateContext(void* window);

		static IGraphicsLibrary& GetLibraryInstance();
		static WindowUserData* GetWindowUserDataPointerStatic(void* window);
	};
}
