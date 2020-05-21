#include <fcntl.h>
#include "Log.h"

namespace GLEN {
	std::shared_ptr<spdlog::logger> Log::Init_spdlog()
	{
		// create color multi threaded logger
		auto console = spdlog::stdout_color_mt("console");

		spdlog::set_level(spdlog::level::debug); // Set global log level to debug
		spdlog::debug("This message should be displayed..");

		// change log pattern
		spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^*-%L-*%$] [thread %t] %v");

		// Compile time log levels
		// define SPDLOG_ACTIVE_LEVEL to desired level
		spdlog::info("Welcome to spdlog!");
		spdlog::error("Some error message with arg: {}", 1);

		spdlog::warn("Easy padding in numbers like {:08d}", 12);
		spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
		spdlog::info("Support for floats {:03.2f}", 1.23456);
		spdlog::info("Positional args are {1} {0}..", "too", "supported");
		spdlog::info("{:<30}", "left aligned");

		return console;
	}

	std::shared_ptr<spdlog::logger> Log::SetupConsole()
	{
		AllocConsole();

		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		const HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long)(handle_in), _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;

		// Test spdlog
		return Init_spdlog();
	}
}