#pragma once
#include "Core.h"
#include "Window.h"
#include "DreamTools/LayerStack.h"
#include "DreamTools/Events/Event.h"
#include "Events/ApplicationEvent.h"
//#include "Events/Event.h"
#include "DreamTools/ImGui/ImGuiLayer.h"

#include "DreamTools/Renderer/Shader.h"
#include "DreamTools/Renderer/Buffer.h"
#include "DreamTools//Renderer/VertexArray.h"

namespace DreamTools 
{ 
	class DREAMTOOLS_API Application
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

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVertexArray;
		
	private:
		static Application* s_Instance;
	};
	//To be defined in a CLIENT
	Application* CreateApplication();
}


