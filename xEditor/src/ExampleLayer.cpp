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
		m_editorCamera = EditorCamera(30.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);

		m_skybox = CreatePtr<Skybox>(std::vector<Ptr<Image>>
		{
			CreatePtr<Image>("asserts/textures/skybox/right.jpg"),
			CreatePtr<Image>("asserts/textures/skybox/left.jpg"),
			CreatePtr<Image>("asserts/textures/skybox/top.jpg"),
			CreatePtr<Image>("asserts/textures/skybox/bottom.jpg"),
			CreatePtr<Image>("asserts/textures/skybox/back.jpg"),
			CreatePtr<Image>("asserts/textures/skybox/front.jpg"),
		});
		m_model = Model::Create("asserts\\models\\vampire\\dancing_vampire.dae");
		m_shader = Shader::Create("asserts\\shaders\\Default.glsl");
		m_shader->Bind();
		int32_t samplers[32];
		for (uint32_t i = 0; i < 32; i++)
		{
			samplers[i] = i;
		}

		m_shader->SetIntArray("uTextures", samplers, 32);

		m_cameraUniformBuffer = UniformBuffer::Create(0, {
				BufferLayoutType::Std140,
				{
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Float3 },
				}
			});

		m_dirLightUniformBuffer = UniformBuffer::Create(1, {
				BufferLayoutType::Std140,
				{
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
				}
			});

		m_dirLightUniformBuffer->SetData(
			{
				glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)),
				glm::value_ptr(glm::vec3(0.5f)),
				glm::value_ptr(glm::vec3(0.4f)),
				glm::value_ptr(glm::vec3(0.5f)),
			});
	}

	void ExampleLayer::OnDetach()
	{
	}

	void ExampleLayer::OnUpdate(Engine::TimeStep timeStep)
	{
		m_editorCamera.OnUpdate(timeStep);
		m_activeScene->OnUpdateEditor(timeStep, m_editorCamera);
		m_model->OnUpdate(timeStep);

		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();

		// view/projection transformations
		m_shader->Bind();
		m_cameraUniformBuffer->SetData({ &m_editorCamera.GetViewMatrix(), &m_editorCamera.GetProjection(), &m_editorCamera.GetPosition()});
		glm::mat4 transform = glm::mat4(1.0f);
		m_shader->SetMat4("uModel", transform);
		m_shader->SetMat3("uInverseModel", glm::transpose(glm::inverse(glm::mat3(transform))));
		std::vector<glm::mat4> transforms = m_model->GetBoneTransforms();
		for (uint32_t i = 0; i < transforms.size(); ++i)
		{
			m_shader->SetMat4("uBoneTransforms[" + std::to_string(i) + "]", transforms[i]);
		}

		// render the loaded model
		m_model->Draw();
		
		// render skybox
		m_skybox->Draw();
	}

	void ExampleLayer::OnImGuiRender()
	{
	}

	void ExampleLayer::OnEvent(Engine::Event& event)
	{
	}
}
