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
	m_IsRunning = true;

	// Initialize the window
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

void DXHEngine::Run()
{
	VS_DB_OUT_W(L"Welcome to DXHEngine! Main loop is starting...\n");
	while (m_IsRunning)
	{
		m_Window->PollEvent();
	}

	Cleanup();
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
