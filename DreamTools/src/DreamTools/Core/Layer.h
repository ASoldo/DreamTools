#pragma once

#include "DreamTools/Core/Core.h"
#include "DreamTools/Core/Timestep.h"
#include "DreamTools/Events/Event.h"

namespace DreamTools
{
	class DREAMTOOLS_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}