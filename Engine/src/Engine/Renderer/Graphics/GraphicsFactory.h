#pragma once
#include "GraphicsAPI/IGraphicsAPI.h"
#include "GraphicsContext/IGraphicsContext.h"
#include "GraphicsLibrary/IGraphicsLibrary.h"
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
