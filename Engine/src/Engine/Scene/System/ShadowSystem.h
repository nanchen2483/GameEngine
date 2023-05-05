#pragma once
#include "ISystem.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"

#include <entt/entt.hpp>

namespace Engine
{
	class ShadowSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry, const glm::mat4& viewMatrix, float FOV, float aspectRatio);
	protected:
		ShadowSystem();
		static ShadowSystem& GetInstance();

		Uniq<ShadowBox> m_shadowBox;
	};
}
