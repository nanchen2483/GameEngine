#include "ExampleLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	ExampleLayer::ExampleLayer()
		: Layer("Example"), m_cameraController(1280.0f / 720.0f, true), m_color(glm::vec3(1.0f, 0.0f, 0.0f))
	{
	}

	void ExampleLayer::OnAttach()
	{
		m_activeScene = CreatePtr<Scene>();
		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_model = Model::Create("asserts\\models\\vampire\\dancing_vampire.dae");
		m_shader = Shader::Create("asserts\\shaders\\Animation.glsl");
		m_shader->Bind();

		int32_t samplers[32];
		for (uint32_t i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}
		m_shader->SetIntArray("uTextures", samplers, 32);
	}

	void ExampleLayer::OnDetach()
	{

	}

	void ExampleLayer::OnUpdate(Engine::TimeStep timeStep)
	{
		m_editorCamera.OnUpdate(timeStep);
		m_model->UpdateAnimation(timeStep);
		m_activeScene->OnUpdateEditor(timeStep, m_editorCamera);

		Renderer2D::ResetStates();
		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();

		// view/projection transformations
		m_shader->Bind();
		m_shader->SetMat4("projection", m_editorCamera.GetProjection());
		m_shader->SetMat4("view", m_editorCamera.GetViewMatrix());

		std::vector<glm::mat4> transforms = m_model->GetPoseTransforms();
		for (int i = 0; i < transforms.size(); ++i)
		{
			m_shader->SetMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		}

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(.5f));	// it's a bit too big for our scene, so scale it down
		m_shader->SetMat4("model", model);
		m_model->Draw();
	}

	void ExampleLayer::OnImGuiRender()
	{
	}

	void ExampleLayer::OnEvent(Engine::Event& event)
	{
		m_cameraController.OnEvent(event);
	}
}
