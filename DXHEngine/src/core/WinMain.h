#pragma once
#include "DXHEngine.h"
#include <Windows.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

// Entry point for Windows applications
int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	DXH::DXHEngine* app = DXH::CreateDXHEngine();
	app->Init();
	app->Run();
	delete app;
	return 0;
}

#else
#error Windows is the only supported platform!
#endif