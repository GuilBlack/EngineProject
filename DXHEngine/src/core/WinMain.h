#pragma once
#include "DXHEngine.h"
#include <Windows.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__CYGWIN__)

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	DXH::DXHEngine* app = DXH::CreateDXHEngine();
	app->Run();
	delete app;
	return 0;
}

#else
#error Windows is the only supported platform!
#endif