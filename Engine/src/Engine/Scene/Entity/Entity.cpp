#include "enginepch.h"
#include "Entity.h"

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
