#include "DXHEngine.h"
#include "Window.h"

namespace DXH
{
bool DXHEngine::Init(AppProperties props, UpdateFunc gameUpdate)
{
	VS_DB_OUT_W(L"Initializing DXHEngine...\n");

	// Initialize the window
	if (!InitWindow())
		return false;

	// Initialize DX12
	if (!InitDX12())
		return false;

	m_GameUpdate = gameUpdate;
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
		Window::GetInstance().PollEvents();
		m_GameTimer.Tick();
		m_GameUpdate(m_GameTimer);
		UpdateFpsCounter(m_GameTimer);
	}

	Cleanup();
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

	if (!Window::GetInstance().Init(winProps, []() { GetInstance().Shutdown(); }))
	{
		VS_DB_OUT_W(L"Failed to initialize the window!\n");
		return false;
	}

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

	Renderer::GetInstance().Init();
	return true;
}

void DXHEngine::UpdateFpsCounter(const Timer& gt)
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;

	if ((m_GameTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float mspf = 1000.0f / frameCnt;
		Window::GetInstance().SetTitle(m_Props.WindowTitle +
			L"    FPS: " + std::to_wstring(frameCnt) +
			L"    MSPF: " + std::to_wstring(mspf));

		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXHEngine::Shutdown()
{
	VS_DB_OUT_W(L"Shutting down DXHEngine...\n");
	m_IsRunning = false;
}

void DXHEngine::Cleanup()
{
	VS_DB_OUT_W(L"Cleaning up DXHEngine...\n");
	DELETE_PTR(m_pContext);
}
}
