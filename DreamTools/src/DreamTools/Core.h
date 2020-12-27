#pragma once

#ifdef DT_PLATFORM_WINDOWS
	#ifdef DT_BUILD_DLL
		#define DREAMTOOLS_API __declspec(dllexport)
	#else
		#define DREAMTOOLS_API __declspec(dllimport)
	#endif
#else
	#error DreamTools only support Windows!
#endif