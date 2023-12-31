#include "DXHEngine.h"
#include "Window.h"
#include "src/ecs/System.h"
#include "src/inputs/InputManager.h"
#include "src/renderer/Renderer.h"
#include "src/time/Timer.h"

namespace DXH
{
DXHEngine::DXHEngine()
    : m_GameTimer(new Timer())
{
}

DXHEngine::~DXHEngine()
{
    DELETE_PTR(m_GameTimer);
}

bool DXHEngine::Init(AppProperties props, GameTimerFunc gameInit, GameTimerFunc gameDestroy)
{
    VS_DB_OUT_W(L"Initializing DXHEngine...\n");

    // Initialize the window
    if (!InitWindow())
        return false;

    // Initialize DX12
    if (!InitDX12())
        return false;

    m_GameInit = gameInit;
    m_GameDestroy = gameDestroy;

    m_IsRunning = true;
    return true;
}

void DXHEngine::Run()
{
    assert(m_IsRunning && "DXHEngine is not initialized!");
    VS_DB_OUT_W(L"Welcome to DXHEngine! Main loop is starting...\n");

    InputManager& im = InputManager::GetInstance();
    m_GameTimer->Reset();
    m_GameInit(*m_GameTimer); // Allow the game to init its game objects
    while (m_IsRunning)
    {
        Window::GetInstance().PollEvents();
        UpdateFpsCounter();
        im.Update();
        m_GameTimer->Tick();

        System::UpdateAll(*m_GameTimer);
        UpdateFpsCounter();
    }

    m_GameDestroy(*m_GameTimer); // Allow the game to delete itself
    Cleanup();
}

void DXHEngine::SetPaused(bool paused)
{
    m_AppPaused = paused;
    if (paused)
        m_GameTimer->Stop();
    else
        m_GameTimer->Start();
}

void DXHEngine::ChangeTimeScale(float timeScale)
{
    if (timeScale <= 0) // Prevent division by zero and negative time scales
        timeScale = FLT_EPSILON;

    m_GameTimer->SetTimeScale(timeScale);
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
    // TODO: Move to Init Renderer
#if defined(DEBUG) || defined(_DEBUG)
    ID3D12Debug* debugController;
    ASSERT_HRESULT(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
    debugController->EnableDebugLayer();
    RELEASE_PTR(debugController);
#endif

    Renderer::GetInstance().Init();

    Renderer::GetInstance().OnResize();

    return true;
}

void DXHEngine::UpdateFpsCounter()
{
    static int frameCnt = 0;
    static float timeElapsed = 0.0f;
    frameCnt++;

    if ((m_GameTimer->TotalTime() - timeElapsed) >= 1.0f)
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
    Renderer::GetInstance().Destroy();
    DELETE_PTR(m_pContext);
}
}
