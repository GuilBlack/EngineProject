#pragma once
#include <pch.h>

namespace DXH
{
// Forward declaration of DXHEngine class
class DXHEngine;
// A pointer to a function that returns void and takes no arguments
typedef void (*WindowCloseCallback)();

// Struct representing the properties of the window
struct WindowProperties
{
	std::wstring Title;
	int32_t PosX = 0, PosY = 0;
	int32_t Width = 0, Height = 0;
	int32_t MinWidth = 0, MinHeight = 0;
};

class Window
{
public:
	Window(WindowProperties props)
		: m_Props(props)
	{
	}
	~Window() {}

	bool Init();
	void Destroy() {}

	// Polls for queued window events
	void PollEvents();
	// Changes the title of the window
	void SetTitle(const std::wstring& title) { SetWindowText(m_WindowHandle, title.c_str()); }

	// Handles window events
	LRESULT OnEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Sets the callback function for when the window is closing
	void SetCloseCallback(WindowCloseCallback callback) { m_CloseCallback = callback; }

	int32_t GetWidth() const noexcept { return m_Props.Width; }
	int32_t GetHeight() const noexcept { return m_Props.Height; }

	HWND GetWindowHandle() const noexcept { return m_WindowHandle; }
private:
	WindowProperties m_Props;
	HWND m_WindowHandle = nullptr;
	WindowCloseCallback m_CloseCallback = nullptr;
};
}

