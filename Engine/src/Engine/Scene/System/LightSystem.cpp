#include "LightSystem.h"

#include "Engine/Scene/Component/LightComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"

namespace Engine
{
	void LightSystem::OnUpdate(entt::registry& registry)
	{
		GetInstance().m_lights.clear();
		registry.view<TransformComponent, LightComponent>()
			.each([&](entt::entity entity, TransformComponent& transform, LightComponent& component)
				{
					component.position = transform.transform.translation;
					GetInstance().m_lights.push_back(component.GetData());
				});
	}

	LightSystem& LightSystem::GetInstance()
	{
		static LightSystem instance;
		return instance;
	}
}
