#pragma once
#include "Window.h"
#include "Timer.h"

namespace DXH
{
// Application properties that are set on startup
struct AppProperties
{
	std::wstring WindowTitle = L"Application - DXH Engine";
	uint16_t WindowWidth = 1280, WindowHeight = 720;
	uint16_t MinWindowWidth = 400, MinWindowHeight = 300;
};

class DXHEngine
{
public:
	DXHEngine(AppProperties props);
	virtual ~DXHEngine() {};

	// Initializes the application
	bool Init();
	// Starts the main loop
	void Run();

	// Gets the singleton application
	static DXHEngine& GetInstance() noexcept { return *s_App; }
	// Gets the window from the singleton application
	static Window& GetWindow() noexcept { return *s_App->m_Window; }

protected:
	// Updates the application
	virtual void Update(const Timer&);

private:
	// Singleton application
	static DXHEngine* s_App;
	Window* m_Window = nullptr;

	AppProperties m_Props;
	Timer m_gameTimer;
	bool m_IsRunning = false;

private:
	// Initializes the window
	bool InitWindow();
	// Initializes DirectX 12
	bool InitDX12();

	// Renders the application
	void Render(const Timer&);

	// Stops the main loop
	void Shutdown();
	// Waits for any operations to finish and destroy the window
	void Cleanup();
};

DXHEngine* CreateDXHEngine();
}

