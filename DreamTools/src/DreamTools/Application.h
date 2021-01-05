#pragma once
#include "Core.h"
#include "Window.h"
#include "DreamTools/LayerStack.h"
#include "DreamTools/Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "DreamTools/Core/Timestep.h"
#include "DreamTools/ImGui/ImGuiLayer.h"

namespace DreamTools 
{ 
	class Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		Ref<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};
	//To be defined in a CLIENT
	Application* CreateApplication();
}


