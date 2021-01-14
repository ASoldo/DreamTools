#include "dtpch.h"
#include "Entity.h"

namespace DreamTools
{
	Entity::Entity(entt::entity handle, Scene* scene)
		:m_EntityHandle(handle), m_Scene(scene)
	{
	}
}