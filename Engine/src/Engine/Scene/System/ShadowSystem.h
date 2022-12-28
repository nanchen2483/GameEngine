#pragma once
#include "ISystem.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"

#include <functional>
#include <entt/entt.hpp>

namespace Engine
{
	class ShadowSystem : public ISystem
	{
	public:
		static void OnUpdate(const glm::mat4& viewMatrix, const float FOV, const float aspectRatio, std::function<void()> OnRender);
		static Ptr<Shader> GetShader() { return GetInstance()->m_shadowBox->GetShader(); }
	protected:
		ShadowSystem();
		static ShadowSystem* GetInstance();

		Uniq<ShadowBox> m_shadowBox;
		static ShadowSystem* s_instance;
	};
}
