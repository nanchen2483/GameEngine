#include "enginepch.h"
#include "ScriptSystem.h"
#include "Engine/Scene/Component/NativeScriptComponent.h"
#include "Engine/Scene/Entity.h"

namespace Engine
{
	void ScriptSystem::OnUpdate(entt::registry& registry, Scene* scene)
	{
		registry.view<NativeScriptComponent>()
			.each([=](entt::entity entity, NativeScriptComponent& nsc)
				{
					if (!nsc.instance)
					{
						nsc.instance = nsc.InstantiateScript();
						nsc.instance->m_entity = Entity{ entity, scene};
						nsc.instance->OnCreate();
					}

					nsc.instance->OnUpdate();
				});
	}
}
