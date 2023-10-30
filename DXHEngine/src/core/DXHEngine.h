#pragma once
#include "Timer.h"
#include "InputManager.h"
#include "src/renderer/Renderer.h"

namespace DXH
{
// Delegate for update functions
typedef void(*UpdateFunc)(const Timer&);

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
	// Gets the singleton application
	inline static DXHEngine& GetInstance() noexcept
	{
		static DXHEngine instance;
		return instance;
	}

	// Initializes the application
	bool Init(AppProperties props, UpdateFunc gameUpdate);
	// Starts the main loop
	void Run();

private:
	AppProperties m_Props;
	Timer m_GameTimer = Timer();
	InputManager m_InputManager = InputManager();
	UpdateFunc m_GameUpdate = nullptr;
	bool m_IsRunning = false;

	RenderContext* m_pContext = nullptr;

private:
	// Private constructor, only the static instance is allowed
	DXHEngine() = default;
	~DXHEngine() = default;

	// Initializes the window
	bool InitWindow();
	// Initializes DirectX 12
	bool InitDX12();

	// Updates the FPS counter
	void UpdateFpsCounter(const Timer&);

	// Stops the main loop
	void Shutdown();
	// Waits for any operations to finish and destroy the window
	void Cleanup();
};
}
