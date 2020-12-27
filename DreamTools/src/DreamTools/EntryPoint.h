#pragma once

#ifdef DT_PLATFORM_WINDOWS

extern DreamTools::Application* DreamTools::CreateApplication();
int main(int argc, char** argv)
{
	auto app = DreamTools::CreateApplication();
	
	app->Run();

	delete app;
}
#endif
