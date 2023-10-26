#pragma once
#include "Window.h"
#include "Timer.h"
#include "../renderer/RenderContext.h"

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
	static Window& GetWindow() noexcept { return *s_App->m_pWindow; }

#pragma region FlagsGettersAndSetters
	bool IsPaused() const noexcept { return m_AppPaused; }
	bool IsMinimized() const noexcept { return m_Minimized; }
	bool IsMaximized() const noexcept { return m_Maximized; }
	bool IsResizing() const noexcept { return m_Resizing; }
	bool IsFullScreen() const noexcept { return m_IsFullScreen; }

	void SetPaused(bool paused) noexcept { m_AppPaused = paused; }
	void SetMinimized(bool minimized) noexcept { m_Minimized = minimized; }
	void SetMaximized(bool maximized) noexcept { m_Maximized = maximized; }
	void SetResizing(bool resizing) noexcept { m_Resizing = resizing; }
	void SetFullScreen(bool fullscreen) noexcept { m_IsFullScreen = fullscreen; }
#pragma endregion

protected:
	// Updates the application
	virtual void Update(const Timer&);

private:
	// Singleton application
	static DXHEngine* s_App;
	Window* m_pWindow = nullptr;

	AppProperties m_Props;
	Timer m_GameTimer = Timer();
	bool m_IsRunning = false;

	RenderContext* m_pContext = nullptr;

#pragma region Flags
	bool m_AppPaused = false;
	bool m_Minimized = false;
	bool m_Maximized = false;
	bool m_Resizing = false;
	bool m_IsFullScreen = false;
#pragma endregion

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

