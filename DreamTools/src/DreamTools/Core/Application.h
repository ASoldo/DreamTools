#pragma once
#include "Core.h"
#include "Window.h"
#include "DreamTools/Core/LayerStack.h"
#include "DreamTools/Events/Event.h"
#include "../Events/ApplicationEvent.h"

#include "DreamTools/Core/Timestep.h"
#include "DreamTools/ImGui/ImGuiLayer.h"

namespace DreamTools 
{ 
	class Application
	{
	public:
		Application(const std::string& name = "Dream Tools");
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		void Close();

		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Ref<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};
	//To be defined in a CLIENT
	Application* CreateApplication();
}


