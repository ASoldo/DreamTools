#include "dtpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
//#include "../vendor/GLFW/include/GLFW/glfw3.h"
#include <glad/glad.h>

#include "Input.h"




namespace DreamTools
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		DT_CORE_ASSERT(!s_Instance, "Application is already running!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		DT_CORE_TRACE("{0}",e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		/*if (e.IsInCategory(EventCategoryApplication))
		{
			DT_CLIENT_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			DT_CLIENT_TRACE(e);
		}*/
		

		while (m_Running)
		{
			glClearColor(0, 1, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			//Debug Draw Mouse Position 
			//auto [x, y] = Input::GetMousePosition();
			//DT_CORE_TRACE("{0} {1}", x, y);

			m_Window->OnUpdate();
		}
	}
}