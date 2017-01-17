#include "Logger\Logger.h"

// library includes
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

namespace engine
{
	void Print(const char* type, const char* format, ...)
	{
		const size_t len_temp = 256;
		char str_temp[len_temp] = { 0 };

		sprintf_s(str_temp, type);
		strcat_s(str_temp, format);
		strcat_s(str_temp, "\n");

		const size_t len_output = len_temp + 1024;
		char str_output[len_output] = { 0 };

		va_list args;
		va_start(args, format);
		vsprintf_s(str_output, len_output, str_temp, args);
		va_end(args);

		OutputDebugStringA(str_output);
	}

#if defined(VERBOSITY_LEVEL) && (VERBOSITY_LEVEL > 1)
	void Print(const char* function_name, const int line_number, const char* format, ...)
	{
		const size_t len_temp = 256;
		char str_temp[len_temp] = { 0 };

		sprintf_s(str_temp, "VERBOSE - %s - %d: ", function_name, line_number);
		strcat_s(str_temp, format);
		strcat_s(str_temp, "\n");

		const size_t len_output = len_temp + 1024;
		char str_output[len_output] = { 0 };

		va_list args;
		va_start(args, format);
		vsprintf_s(str_output, len_output, str_temp, args);
		va_end(args);

		OutputDebugStringA(str_output);
	}
#endif // defined(DEBUG_LOG_LEVEL) && (DEBUG_LOG_LEVEL > 1)

} // namespace engine