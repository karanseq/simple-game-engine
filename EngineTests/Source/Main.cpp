// library includes
#include <conio.h>
#include <stdio.h>
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// engine includes
#include "Common\Engine.h"
#include "Logger\Logger.h"

void RunTests();

int main(int* argv, char** argc)
{
	printf("Running engine tests.\nCheck Output window for details.");

	engine::StartUp();

	RunTests();
	LOG("\n");

	engine::Shutdown();

#if defined(BUILD_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	
	printf("\nFinished engine tests.\nPress any key to exit...");
	_getch();

	return 0;
}

