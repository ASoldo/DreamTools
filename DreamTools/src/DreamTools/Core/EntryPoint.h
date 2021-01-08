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
		
		DT_PROFILE_BEGIN_SESSION("Startup", "DreamToolsProfile-Startup.json");
		auto app = DreamTools::CreateApplication();
		DT_PROFILE_END_SESSION();

		DT_PROFILE_BEGIN_SESSION("Runtime", "DreamToolsProfile-Runtime.json");
		app->Run();
		DT_PROFILE_END_SESSION();

		DT_PROFILE_BEGIN_SESSION("Shutdown", "DreamToolsProfile-Shutdown.json");
		delete app;
		DT_PROFILE_END_SESSION();
	}
#endif
