#include "dtpch.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include "DreamTools/Renderer/Renderer2D.h"

namespace DreamTools
{
	/*static void DoStuff(const glm::mat4& transform)
	{

	}*/

	/*static void OnTransformConstructed(entt::registry& registry, entt::entity ent)
	{

	}*/

	Scene::Scene()
	{
		//struct  TransformComponent
		//{
		//	glm::mat4 Transform;
		//	TransformComponent() = default;
		//	TransformComponent(const TransformComponent&) = default;
		//	TransformComponent(const glm::mat4& transform) : Transform(transform) {}
		//	//implicite cast operator
		//	operator glm::mat4&() { return Transform; }
		//	operator const glm::mat4&() const { return Transform; }
		//};
		//struct SpriteComponent
		//{
		//};
		///*TransformComponent transform;
		//DoStuff(transform);*/
		//entt::entity ent = m_Registry.create();
		//auto& transform = m_Registry.emplace<TransformComponent>(ent, glm::mat4(1.0f));
		////m_Registry.remove<TransformComponent>(ent);
		////m_Registry.clear();
		////transform = glm::mat4(1.0f);
		////TransformComponent& transformResult = m_Registry.get<TransformComponent>(ent);
		////if(m_Registry.has<TransformComponent>(ent))
		////auto view = m_Registry.view<TransformComponent>();
		////for (auto entity : view)
		////{
		////	view.get<TransformComponent>(ent);
		////	TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		////}
		//m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstructed>();
		//auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		//for (auto entity : group)
		//{
		//	group.get<TransformComponent>(ent);
		//	//auto&[transform, mesh] = group.get<TransformComponent, SpriteComponent>(entity);
		//}
	}
	Scene::~Scene()
	{
	}
	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto&[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}