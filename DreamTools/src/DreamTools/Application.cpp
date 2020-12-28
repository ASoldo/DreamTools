#include "Application.h"
#include "Events/ApplicationEvent.h"
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
		WindowResizeEvent e(1024, 768);
		DT_CLIENT_TRACE(e);
		while (true)
		{

		}
	}
}