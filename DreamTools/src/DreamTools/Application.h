#pragma once
#include "Core.h"
#include "Window.h"
//#include "Events/Event.h"

namespace DreamTools 
{ 
	class DREAMTOOLS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	private:
		std::unique_ptr<Window> my_Window;
		bool m_Running = true;
	};
	//To be defined in a CLIENT
	Application* CreateApplication();
}


