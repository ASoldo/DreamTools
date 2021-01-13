#pragma once
#include "entt.hpp"
#include "DreamTools/Core/Timestep.h"
#include "DreamTools/Scene/Components.h"

namespace DreamTools
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);

		//Temp
		entt::registry& Reg() { return m_Registry; }

		entt::entity CreateEntity();
	private:
		entt::registry m_Registry;

	};
}