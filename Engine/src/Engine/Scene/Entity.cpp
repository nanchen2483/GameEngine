#include "enginepch.h"
#include "Entity.h"

#include "Component/AnimationComponent.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/MeshComponent.h"
#include "Component/NativeScriptComponent.h"
#include "Component/PhysicsComponent.h"
#include "Component/SkyboxComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TransformComponent.h"
#include "Component/TagComponent.h"
#include "Component/TerrainComponent.h"

namespace Engine
{
	Entity::Entity()
		: Entity(entt::null, nullptr)
	{
	}

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_entityHandle(handle), m_scene(scene)
	{
	}

	template<typename T, typename std::enable_if_t<std::is_base_of_v<IComponent, T>>*>
	void Entity::OnComponentAdded(T& component)
	{
		static_assert(false);
	}

	template<>
	void Entity::OnComponentAdded<TagComponent>(TagComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<TransformComponent>(TransformComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<CameraComponent>(CameraComponent& component)
	{
		component.camera.SetViewportSize(m_scene->m_viewportWidth, m_scene->m_viewportHeight);
	}

	template<>
	void Entity::OnComponentAdded<SpriteRendererComponent>(SpriteRendererComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<LightComponent>(LightComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<MeshComponent>(MeshComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<AnimationComponent>(AnimationComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<PhysicsComponent>(PhysicsComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<SkyboxComponent>(SkyboxComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<TerrainComponent>(TerrainComponent& component)
	{
	}

	template<>
	void Entity::OnComponentAdded<NativeScriptComponent>(NativeScriptComponent& component)
	{
	}

	Entity::operator bool() const
	{
		return m_entityHandle != entt::null && m_scene != nullptr && m_scene->EntityExists(m_entityHandle);
	}

	bool Entity::operator==(const Entity& other) const
	{
		return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
	}

	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	bool Entity::operator<(const Entity& other) const
	{
		ENGINE_CORE_ERROR("operator < not valid");

		return false;
	}

	bool Entity::operator<=(const Entity& other) const
	{
		ENGINE_CORE_ERROR("operator <= not valid");

		return false;
	}

	bool Entity::operator>(const Entity& other) const
	{
		ENGINE_CORE_ERROR("operator > not valid");

		return false;
	}

	bool Entity::operator>=(const Entity& other) const
	{
		ENGINE_CORE_ERROR("operator >= not valid");

		return false;
	}
}
