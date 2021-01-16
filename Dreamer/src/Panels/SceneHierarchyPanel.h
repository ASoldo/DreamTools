#pragma once
#include "DreamTools/Core/Log.h"
#include "DreamTools/Core/Core.h"
#include "DreamTools/Scene/Scene.h"
#include "DreamTools/Scene/Entity.h"

namespace DreamTools
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();
		Entity GetSelectedEntity() const { return m_SelectionContext; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};
}