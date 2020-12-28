#pragma once
#include "Log.h"

#ifdef DT_PLATFORM_WINDOWS

	extern DreamTools::Application* DreamTools::CreateApplication();
	int main(int argc, char** argv)
	{
		DreamTools::Log::Init();
		DT_CORE_WARN("DreamTools Initialized!");
		DT_CLIENT_INFO("App Initialized!");
		DT_CORE_WARN(DT_USERNAME);
		
		auto app = DreamTools::CreateApplication();
	
		app->Run();

		delete app;
	}
#endif
