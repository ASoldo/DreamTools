#pragma once
#include "entt.hpp"

namespace DreamTools
{
	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;

	};
}