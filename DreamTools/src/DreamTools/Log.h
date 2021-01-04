#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace DreamTools
{
	class DREAMTOOLS_API Log
	{
	public:
		Log();
		~Log();
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define DT_USERNAME "Welcome, Aetrix!"
//CORE LOG MACROS
#define DT_CORE_ERROR(...)   ::DreamTools::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DT_CORE_WARN(...)    DreamTools::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DT_CORE_INFO(...)    ::DreamTools::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DT_CORE_TRACE(...)   ::DreamTools::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DT_CORE_FATAL(...)   ::DreamTools::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client LOG MACROS
#define DT_CLIENT_ERROR(...)   ::DreamTools::Log::GetClientLogger()->error(__VA_ARGS__)
#define DT_CLIENT_WARN(...)    ::DreamTools::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DT_CLIENT_INFO(...)    ::DreamTools::Log::GetClientLogger()->info(__VA_ARGS__)
#define DT_CLIENT_TRACE(...)   ::DreamTools::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DT_CLIENT_FATAL(...)   ::DreamTools::Log::GetClientLogger()->fatal(__VA_ARGS__)