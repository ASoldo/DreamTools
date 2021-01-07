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
		virtual void OnImGuiRender() override;
		//virtual void OnUpdate() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;

	};
}