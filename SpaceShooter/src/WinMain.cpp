#pragma once
#include <Windows.h>
#include "Game.h"

#if defined(DEBUG) | defined(_DEBUG)
#include <crtdbg.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

// Entry point for Windows applications
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	_CrtMemState memState;
	_CrtMemCheckpoint(&memState);
#endif

	Game::GetInstance().StartEngine();

# if defined(DEBUG) | defined(_DEBUG)
	_CrtMemState memState2, memStateDiff;
	_CrtMemCheckpoint(&memState2);
	if (_CrtMemDifference(&memStateDiff, &memState, &memState2))
	{
		MessageBox(nullptr,
			L"Memory leak detected!\nCheck the output window for more information.",
			L"Memory leak!",
			MB_OK | MB_ICONERROR);

		// Dump memory leaks to output window
		_CrtMemDumpStatistics(&memStateDiff);
		_CrtDumpMemoryLeaks();
		return 1;
	}
#endif

	return 0;
}

#else
#error Windows is the only supported platform!
#endif