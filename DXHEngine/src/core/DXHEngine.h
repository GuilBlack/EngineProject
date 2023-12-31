#pragma once
#include <string>

namespace DXH
{
class Timer;
class RenderContext;

// Delegate for update functions
typedef void(*GameTimerFunc)(const Timer&);

// Application properties that are set on startup
struct AppProperties
{
    std::wstring WindowTitle = L"Application - DXH Engine";
    uint16_t WindowWidth = 1280, WindowHeight = 720;
    uint16_t MinWindowWidth = 400, MinWindowHeight = 300;
};

class DXHEngine
{
    // Private constructor, only the static instance is allowed
    DXHEngine();
    ~DXHEngine();

public:
    // Gets the singleton application
    inline static DXHEngine& GetInstance() noexcept
    {
        static DXHEngine instance;
        return instance;
    }

    // Initializes the application
    bool Init(AppProperties props, GameTimerFunc gameInit, GameTimerFunc gameDestroy);
    // Starts the main loop
    void Run();

#pragma region FlagsGettersAndSetters
    bool IsPaused() const noexcept { return m_AppPaused; }
    bool IsMinimized() const noexcept { return m_Minimized; }
    bool IsMaximized() const noexcept { return m_Maximized; }
    bool IsResizing() const noexcept { return m_Resizing; }
    bool IsFullScreen() const noexcept { return m_IsFullScreen; }

    void SetPaused(bool paused);
    void SetMinimized(bool minimized) noexcept { m_Minimized = minimized; }
    void SetMaximized(bool maximized) noexcept { m_Maximized = maximized; }
    void SetResizing(bool resizing) noexcept { m_Resizing = resizing; }
    void SetFullScreen(bool fullscreen) noexcept { m_IsFullScreen = fullscreen; }
#pragma endregion

    void ChangeTimeScale(float timeScale);

    // Stops the main loop
    void Shutdown();

protected:
    // Updates the application
    virtual void Update(const Timer&) {};

private:
    AppProperties m_Props;
    Timer* m_GameTimer = nullptr;
    GameTimerFunc m_GameInit = nullptr;
    GameTimerFunc m_GameDestroy = nullptr;
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

    // Updates the FPS counter
    void UpdateFpsCounter();

    // Waits for any operations to finish and destroy the window
    void Cleanup();
};
}
