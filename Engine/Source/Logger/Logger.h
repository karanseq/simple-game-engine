#ifndef ENGINE_LOGGER_H_
#define ENGINE_LOGGER_H_

/*
	A simple logging utility.
	Features:
	- Supports a simple log function in both debug and release modes at various verbosity levels (see Variables section)

	Variables:
	- BUILD_DEBUG: Define this only when we're in debug mode.
	- VERBOSITY_LEVEL
		- In RELEASE mode, set VERBOSITY_LEVEL to 1 to enable logs, 0 to disable all logs
			VERBOSITY_LEVEL = 0
				- Log & error are disabled
			VERBOSITY_LEVEL = 1
				- Log & error are enabled

		- In DEBUG mode,
			VERBOSITY_LEVEL = 0
				- Log & error are enabled, verbose is disabled
			VERBOSITY_LEVEL = 1
				- Log, error & verbose are enabled
*/

#if defined(BUILD_DEBUG)
	// in debug mode, enable verbose, log & error when level is 1
	#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 0)
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
		#define VERBOSE(format, ...)		engine::Print("VERBOSE: ", (format), __VA_ARGS__)
	#else
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
		#define VERBOSE(format, ...)		void(0)
	#endif
#else
	// in release mode, disable verbose
	#define VERBOSE(format, ...)			void(0)

	// in release mode, enable log & error when level is 1
	#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 0)
		#define LOG(format, ...)			engine::Print("DEBUG: ", (format), __VA_ARGS__)
		#define LOG_ERROR(format, ...)		engine::Print("ERROR: ", (format), __VA_ARGS__)
	#else
		#define LOG(format, ...)			void(0)
		#define LOG_ERROR(format, ...)		void(0)
	#endif
#endif

namespace engine
{
	void Print(const char* i_type, const char* i_format, ...);
#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 1)
	void Print(const char* i_function_name, const int i_line_number, const char* i_format, ...);
#endif
} // namespace engine

#endif // ENGINE_LOGGER_H_