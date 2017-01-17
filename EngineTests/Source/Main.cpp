// library includes
#include <conio.h>
#include <stdio.h>
#ifdef BUILD_DEBUG
#include <crtdbg.h>
#endif // BUILD_DEBUG

// engine includes
#include "Logger\Logger.h"
#include "Memory\AllocatorUtil.h"

void RunTests();

int main(int* argv, char** argc)
{
	printf("Running engine tests.\nCheck Output window for details.");

	engine::memory::CreateAllocators();

	RunTests();
	LOG("\n");

	engine::memory::DestroyAllocators();

#if defined(BUILD_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
	
	printf("\nFinished engine tests.\nPress any key to exit...");
	_getch();

	return 0;
}

