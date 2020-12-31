#include <DreamTools.h>
#include "DreamTools/ImGui/ImGuiLayer.h"

class ExampleLayer : public DreamTools::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}
	void OnUpdate() override
	{
		//DT_CORE_INFO("ExampleLayer::OnUpdate");

		if (DreamTools::Input::IsKeyPressed(DreamTools::Key::Tab))
		{
			DT_CORE_TRACE("Tab is pressed! (POLL)");
		}
	}

	void OnEvent(DreamTools::Event& event) override
	{
		if (event.GetEventType() == DreamTools::EventType::KeyPressed)
		{
			DreamTools::KeyPressedEvent& e = (DreamTools::KeyPressedEvent&)event;
			if (e.GetKeyCode() == DreamTools::Key::Tab)
			{
				DT_CORE_TRACE("{0} is pressed! (EVENT)", (char)e.GetKeyCode());
			}

			DT_CORE_TRACE("{0}", (char)e.GetKeyCode());
		}

		//DT_CORE_TRACE("{0}", event);
	}
};

class Sandbox : public DreamTools::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new DreamTools::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};

DreamTools::Application* DreamTools::CreateApplication()
{
	return new Sandbox;
}