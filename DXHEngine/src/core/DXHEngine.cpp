#include "DXHEngine.h"

namespace DXH
{
DXHEngine* DXHEngine::s_App = nullptr;

DXHEngine::DXHEngine(AppProperties props)
	: m_Props(props)
{
	assert(s_App == nullptr && "There should only be one instance of the app running!");
	s_App = this;
}

bool DXHEngine::Init()
{
	VS_DB_OUT_W(L"Initializing DXHEngine...\n");

	// Initialize the window
	if (!InitWindow())
		return false;

	// Initialize DX12
	if (!InitDX12())
		return false;

	m_IsRunning = true;
	return true;
}

void DXHEngine::Run()
{
	assert(m_IsRunning && "DXHEngine is not initialized!");
	VS_DB_OUT_W(L"Welcome to DXHEngine! Main loop is starting...\n");

	m_gameTimer.Reset();
	while (m_IsRunning)
	{
		m_Window->PollEvents();
		m_gameTimer.Tick();
		Update(m_gameTimer);
		Render(m_gameTimer);
	}

	Cleanup();
}

void DXHEngine::Update(const Timer&)
{
	// Do nothing. This method should be overrided by the user.
}

bool DXHEngine::InitWindow()
{
	WindowProperties winProps
	{
		.Title = m_Props.WindowTitle,
		.Width = (int32_t)m_Props.WindowWidth,
		.Height = (int32_t)m_Props.WindowHeight,
		.MinWidth = (int32_t)m_Props.MinWindowWidth,
		.MinHeight = (int32_t)m_Props.MinWindowHeight,
	};

	m_Window = new Window(winProps);
	if (!m_Window->Init())
	{
		VS_DB_OUT_W(L"Failed to initialize the window!\n");
		return false;
	}
	m_Window->SetCloseCallback([]() { GetInstance().Shutdown(); });

	return true;
}

bool DXHEngine::InitDX12()
{
	return true;
}

void DXHEngine::Render(const Timer& gt)
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;

	if ((m_gameTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float mspf = 1000.0f / frameCnt;
		m_Window->SetTitle(m_Props.WindowTitle +
			L"    FPS: " + std::to_wstring(frameCnt) +
			L"    MSPF: " + std::to_wstring(mspf));

		frameCnt = 0;
		timeElapsed += 1.0f;
	}

	// TODO after implementing InitDX12()
}

void DXHEngine::Shutdown()
{
	VS_DB_OUT_W(L"Shutting down DXHEngine...\n");
	m_IsRunning = false;
}

void DXHEngine::Cleanup()
{
	VS_DB_OUT_W(L"Cleaning up DXHEngine...\n");
	delete m_Window;
}
}
