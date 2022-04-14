#include "ExampleLayer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	ExampleLayer::ExampleLayer()
		: Layer("Example")
	{
	}

	void ExampleLayer::OnAttach()
	{
		m_activeScene = CreatePtr<Scene>();
		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_model = Model::Create("asserts\\models\\vampire\\dancing_vampire.dae");
		m_shader = Shader::Create("asserts\\shaders\\Default.glsl");
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
		m_shader->SetBool("uEnableAnimation", true);
		m_shader->SetBool("uUseModel", true);
		m_shader->SetMat4("uViewProjection", m_editorCamera.GetProjection() * m_editorCamera.GetViewMatrix());
		std::vector<glm::mat4> transforms = m_model->GetBoneTransforms();
		for (int i = 0; i < transforms.size(); ++i)
		{
			m_shader->SetMat4("uFinalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
		}

		m_shader->SetFloat3("uViewPos", m_editorCamera.GetPosition());

		// light properties
		float currentFrame = static_cast<float>(timeStep) * 10;
		ENGINE_CORE_INFO("currentFrame: {0}", currentFrame);
		m_shader->SetFloat3("uDirLight.position", glm::vec3(currentFrame, 1.0f, currentFrame));
		m_shader->SetFloat3("uDirLight.ambient", glm::vec3(0.4f));
		m_shader->SetFloat3("uDirLight.diffuse", glm::vec3(0.5f));
		m_shader->SetFloat3("uDirLight.specular", glm::vec3(1.0f));

		// render the loaded model
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		m_shader->SetMat4("uModel", model);
		m_model->Draw();
	}

	void ExampleLayer::OnImGuiRender()
	{
	}

	void ExampleLayer::OnEvent(Engine::Event& event)
	{
	}
}
