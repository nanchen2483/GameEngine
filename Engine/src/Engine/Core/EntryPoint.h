#pragma once
#include "Engine/Core/Base.h"

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

void main(int argc, char** argv)
{
	Engine::Log::Init();

	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Engine_Startup.json");
	auto app = Engine::CreateApplication();
	ENGINE_PROFILE_END_SESSION();
	
	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Engine_Runtime.json");
	app->Run();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Engine_Shutdown.json");
	delete app;
	ENGINE_PROFILE_END_SESSION();
}

#endif // ENGINE_PLATFORM_WINDOWS
