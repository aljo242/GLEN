#pragma once

#include "GLEN_PCH.h"

namespace GLEN
{
	class Log {
	public:
		Log()
		{
			m_Logger = SetupConsole();
		}

	private:
		std::shared_ptr<spdlog::logger> m_Logger;
		std::shared_ptr<spdlog::logger> Init_spdlog();
		std::shared_ptr<spdlog::logger> SetupConsole();
	};
}

// USE MACROS to expand logger

#ifdef _DEBUG
#define GLEN_TRACE(...)		spdlog::trace(__VA_ARGS__)
#define GLEN_INFO(...)			spdlog::info(__VA_ARGS__)
#define GLEN_WARN(...)			spdlog::warn(__VA_ARGS__)
#define GLEN_ERROR(...)		spdlog::error(__VA_ARGS__)
#define GLEN_CRITICAL(...)		spdlog::critical(__VA_ARGS__)
#endif

#ifndef _DEBUG
#define GLEN_TRACE(...)		{ }
#define GLEN_INFO(...)			{ }
#define GLEN_WARN(...)			{ }
#define GLEN_ERROR(...)		{ }
#define GLEN_CRITICAL(...)		{ }
#endif