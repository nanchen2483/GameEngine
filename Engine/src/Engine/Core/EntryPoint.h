#pragma once
#include "Base.h"
#include "Engine/Debug/Instrumentor.h"

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv)
{
	Engine::Log::Init();

	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Engine_Startup.json");
	Engine::Application* app = Engine::CreateApplication();
	ENGINE_PROFILE_END_SESSION();
	
	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Engine_Runtime.json");
	app->Run();
	ENGINE_PROFILE_END_SESSION();

	ENGINE_PROFILE_BEGIN_SESSION("Startup", "Engine_Shutdown.json");
	delete app;
	ENGINE_PROFILE_END_SESSION();

	return 0;
}
