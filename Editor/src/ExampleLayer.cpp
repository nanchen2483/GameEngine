#include "ExampleLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Engine
{
	ExampleLayer::ExampleLayer()
		: Layer("Example")
	{
	}

	void ExampleLayer::OnAttach()
	{
		m_editorCamera = EditorCamera(30.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);
		m_terrain = ITerrain::Create(TerrainType::Default, "assets/textures/heights/iceland_heightmap.png");

		m_cameraUniformBuffer = UniformBuffer::Create(0, {
			BufferLayoutType::Std140,
			{
				{ ShaderDataType::Mat4 },
				{ ShaderDataType::Mat4 },
				{ ShaderDataType::Float3 },
			}
		});
	}

	void ExampleLayer::OnDetach()
	{
	}

	void ExampleLayer::OnUpdate()
	{
		m_editorCamera.OnUpdate();

		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();

		m_cameraUniformBuffer->SetData({ &m_editorCamera.GetViewMatrix(), &m_editorCamera.GetProjection(), &m_editorCamera.GetPosition() });
		m_terrain->Draw(glm::mat4(1.0f), { {}, {}, 0, 0, 0, 0 });
	}

	void ExampleLayer::OnImGuiRender()
	{
	}

	void ExampleLayer::OnEvent(Engine::Event& event)
	{
	}
}
