#pragma once
#include "Engine/Core/System/Object/IComparable.h"
#include "Engine/Scene/Component/IComponent.h"
#include "Engine/Scene/Scene.h"

#include <entt/entt.hpp>
#include <type_traits>

namespace Engine
{
	class Entity : public IComparable<Entity>
	{
	public:
		Entity();
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		uint32_t GetId() const { return (uint32_t)m_entityHandle; }

		template<typename T, typename std::enable_if_t<std::is_base_of_v<IComponent, T>>* = nullptr>
		T& GetComponent()
		{
			ENGINE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return m_scene->m_registry.get<T>(m_entityHandle);
		}

		template<typename T, typename std::enable_if_t<std::is_base_of_v<IComponent, T>>* = nullptr>
		bool HasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_entityHandle);
		}

		template<typename T, typename... Args, typename std::enable_if_t<std::is_base_of_v<IComponent, T>>* = nullptr>
		T& AddComponent(Args&&... args)
		{
			ENGINE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& componet = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			OnComponentAdded<T>(componet);

			return componet;
		}

		template<typename T, typename std::enable_if_t<std::is_base_of_v<IComponent, T>>* = nullptr>
		void RemoveComponent()
		{
			ENGINE_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			m_scene->m_registry.remove<T>(m_entityHandle);
		}

		operator entt::entity() const { return m_entityHandle; }
		operator int32_t() const { return (int32_t)m_entityHandle; }
		operator uint32_t() const { return (uint32_t)m_entityHandle; }
		operator uint64_t() const { return (uint64_t)m_entityHandle; }
		operator bool() const;

		// Inherited via IComparable
		virtual bool operator==(const Entity& other) const override;
		virtual bool operator!=(const Entity& other) const override;
		virtual bool operator<(const Entity& other) const override;
		virtual bool operator<=(const Entity& other) const override;
		virtual bool operator>(const Entity& other) const override;
		virtual bool operator>=(const Entity& other) const override;
	private:
		template<typename T, typename std::enable_if_t<std::is_base_of_v<IComponent, T>>* = nullptr>
		void OnComponentAdded(T& component);

		entt::entity m_entityHandle;
		Scene* m_scene;
	};
}
