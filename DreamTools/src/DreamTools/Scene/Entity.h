#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace DreamTools
{
	class Entity
	{
	public:
		Entity() = default;
		//virtual ~Entity();
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		//Variatic template, dont unpack parameters here, just pass them to entt
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			DT_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			DT_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemmoveComponent()
		{
			DT_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}
		operator bool() const { return m_EntityHandle != entt::null; }

	private:
		entt::entity m_EntityHandle { entt::null };
		Scene* m_Scene = nullptr; //12 bites on 64 bit Build

	};
}