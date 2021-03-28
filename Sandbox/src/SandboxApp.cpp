#pragma once

#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		ENGINE_INFO("ExampleLayer::Update");
	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(Engine::Event& event) override
	{
		ENGINE_TRACE("{0}", event);
	}
};


class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}