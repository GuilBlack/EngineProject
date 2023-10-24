#include "DXHEngine.h"
#include "../renderer/Renderer.h"

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

	m_GameTimer.Reset();
	while (m_IsRunning)
	{
		m_pWindow->PollEvents();
		m_GameTimer.Tick();
		Update(m_GameTimer);
		Render(m_GameTimer);
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

	m_pWindow = new Window(winProps);
	if (!m_pWindow->Init())
	{
		VS_DB_OUT_W(L"Failed to initialize the window!\n");
		return false;
	}
	m_pWindow->SetCloseCallback([]() { GetInstance().Shutdown(); });

	return true;
}

bool DXHEngine::InitDX12()
{
#if defined(DEBUG) || defined(_DEBUG)
	ID3D12Debug* debugController;
	ASSERT_HRESULT(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	RELEASE_PTR(debugController);
#endif

	Renderer::GetInstance().Init(m_pWindow);
	return true;
}

void DXHEngine::Render(const Timer& gt)
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;

	if ((m_GameTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float mspf = 1000.0f / frameCnt;
		m_pWindow->SetTitle(m_Props.WindowTitle +
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
	delete m_pContext;
	delete m_pWindow;
}
}
