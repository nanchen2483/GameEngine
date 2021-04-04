#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

void main(int argc, char** argv)
{
	Engine::Log::Init();
	ENGINE_CORE_WARN("Initialized Log!");
	ENGINE_INFO("Hello");
	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#endif // ENGINE_PLATFORM_WINDOWS
