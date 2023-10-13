#pragma once
#include "Window.h"

namespace DXH 
{
	// Application properties that are set on startup
	struct AppProperties
	{
		std::wstring WindowTitle = L"DXH Engine";
		int32_t WindowWidth = 1280, WindowHeight = 720;
		int32_t MinWindowWidth = 400, MinWindowHeight = 300;
	};

	class DXHEngine 
	{
	public:
		DXHEngine(AppProperties props);
		virtual ~DXHEngine() {}
		bool Init();
		void Run();

		// Gets the singleton application
		static DXHEngine& GetInstance() noexcept { return *s_App; }
		// Gets the window from the singleton application
		static Window& GetWindow() noexcept { return *GetInstance().m_Window; }

	private:
		// Singleton application
		static DXHEngine* s_App;
		Window* m_Window = nullptr;

		AppProperties m_Props;
		bool m_IsRunning = true;


	private:
		void Shutdown();
	};

	DXHEngine* CreateDXHEngine();
}

