#pragma once
#include <string>

namespace DXH
{
// Struct representing the properties of the window
struct WindowProperties
{
    std::wstring Title;
    int32_t PosX = 0, PosY = 0;
    int32_t Width = 0, Height = 0;
    int32_t MinWidth = 0, MinHeight = 0;
};

// Function to be called when the window is closed
typedef void(*WindowCloseCallback)();

class Window
{
public:
    inline static Window& GetInstance()
    {
        static Window instance;
        return instance;
    }
    /// <summary>
    /// Initializes the window.
    /// </summary>
    /// <param name="props">Properties of the window</param>
    /// <param name="callback">Callback function to be called when the window is closed</param>
    bool Init(WindowProperties props, WindowCloseCallback callback);
    /// <summary>
    /// Destroys the window.
    /// </summary>
    void Destroy();
    /// <summary>
    /// Computes all the events that happened since the last call.
    /// </summary>
    void PollEvents();
    /// <summary>
    /// Sets the title of the window.
    /// </summary>
    void SetTitle(const std::wstring& title) { SetWindowText(m_WindowHandle, title.c_str()); }

    int32_t GetWidth() const noexcept { return m_Props.Width; }
    int32_t GetHeight() const noexcept { return m_Props.Height; }
    HWND GetWindowHandle() const noexcept { return m_WindowHandle; }

private:
    Window() = default;
    ~Window() = default;

    // Handles window events
    static LRESULT WINAPI OnEvent(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    WindowProperties m_Props;
    HWND m_WindowHandle = nullptr;
    WindowCloseCallback m_CloseCallback = nullptr;
};
}
