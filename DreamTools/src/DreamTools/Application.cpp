#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Log.h"

namespace DreamTools
{

	Application::Application()
	{
		
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		DT_CORE_TRACE(e);

		while (true)
		{

		}
	}
}