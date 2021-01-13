#pragma once
#include "DreamTools/Core/Layer.h"
#include "DreamTools/Events/KeyEvent.h"
#include "DreamTools/Events/MouseEvent.h"
#include "DreamTools/Events/ApplicationEvent.h"

namespace DreamTools
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;

	};
}