#include "ShadowSystem.h"
#include "Engine/Scene/Component/ModelComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"

namespace Engine
{
	ShadowSystem* ShadowSystem::s_instance = nullptr;
	ShadowSystem::ShadowSystem()
	{
		m_shadowBox = CreateUniq<ShadowBox>();
	}

	void ShadowSystem::OnUpdate(const glm::mat4& viewMatrix, const float FOV, const float aspectRatio, std::function<void()> OnRender)
	{
		GetInstance()->m_shadowBox->Update(viewMatrix, FOV, aspectRatio);
		GetInstance()->m_shadowBox->Bind();
		OnRender();
		GetInstance()->m_shadowBox->Ubind();
		GetInstance()->m_shadowBox->BindTexture();
	}

	ShadowSystem* ShadowSystem::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new ShadowSystem();
		}

		return s_instance;
	}
}
