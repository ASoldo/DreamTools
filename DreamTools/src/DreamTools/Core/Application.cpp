#include "dtpch.h"
#include "Application.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/Event.h"

#include <glad/glad.h>
#include "DreamTools/Renderer/Renderer.h"
#include "DreamTools/Renderer/RenderCommand.h"

#include "../GLFW/include/GLFW/glfw3.h"
#include "Input.h"

namespace DreamTools
{
	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		DT_PROFILE_FUNCTION();
		DT_CORE_ASSERT(!s_Instance, "Application is already running!");

		s_Instance = this;
		m_Window = Scope<Window>(Window::Create(WindowProps(name)));
		
		//Resize Console App
		HWND console = GetConsoleWindow();
		MoveWindow(console, 0, 0, 800, 500, TRUE); //MoveWindow(window_handle, x, y, width, height, redraw_window);
		WindowResizeEvent e(1280, 720);

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
		Renderer::Init();
		m_Window->SetVSync(true);
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		DT_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
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

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		DT_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		DT_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		DT_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	Application::~Application()
	{
		DT_PROFILE_FUNCTION();
	}

	void Application::Run()
	{
		DT_PROFILE_FUNCTION();

		///Main game Loop
		while (m_Running)
		{
			DT_PROFILE_SCOPE("Main Game Loop");

			float time = (float)glfwGetTime(); //Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					DT_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(timestep);
					}
				}
			}

			m_ImGuiLayer->Begin();
			{
				DT_PROFILE_SCOPE("LayerStack OnImGuiRender");

				for (Layer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}