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

	private:
		void DrawEntityNode(Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

	};
}