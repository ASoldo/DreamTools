#pragma once
#include "entt.hpp"
#include "DreamTools/Core/Timestep.h"
#include "DreamTools/Scene/Components.h"

namespace DreamTools
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);

		Entity CreateEntity(const std::string& name = std::string());

	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}