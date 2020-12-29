#pragma once

#ifdef DT_PLATFORM_WINDOWS
	#ifdef DT_BUILD_DLL
		#define DREAMTOOLS_API __declspec(dllexport)
	#else
		#define DREAMTOOLS_API __declspec(dllimport)
	#endif
#else
	#error DreamTools only supports Windows OS!
#endif

#ifdef DT_ENABLE_ASSERTS
	#define DT_CLIENT_ASSERT(x,...) { if(!(x)) { DT_CLIENT_ERROR("Assertion Failed: {0}", ___VA_ARGS__); __debugbreak();}}
	#define DT_CORE_ASSERT(x,...) { if(!(x)) { DT_CORE_ERROR("Assertion Failed: {0}", ___VA_ARGS__); __debugbreak();}}
#else
	#define DT_CLIENT_ASSERT(x, ...)
	#define DT_CORE_ASSERT(x, ...)
#endif