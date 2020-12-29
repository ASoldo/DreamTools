#include "dtpch.h"
#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Log.h"

namespace DreamTools
{

	Application::Application()
	{
		my_Window = std::unique_ptr<Window>(Window::Create());
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
			my_Window->OnUpdate();
		}
	}
}