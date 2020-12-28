#pragma once
#include "Log.h"

#ifdef DT_PLATFORM_WINDOWS

	extern DreamTools::Application* DreamTools::CreateApplication();
	int main(int argc, char** argv)
	{
		DreamTools::Log::Init();
		DreamTools::Log::GetCoreLogger()->warn("DreamTools Initialized!");
		DreamTools::Log::GetClientLogger()->info("App Initialized!");
		DreamTools::Log::GetClientLogger()->info(DT_USERNAME);
		

		auto app = DreamTools::CreateApplication();
	
		app->Run();

		delete app;
	}
#endif
