#include "Window.h"

namespace DXH
{
bool Window::Init(WindowProperties props, WindowCloseCallback callback)
{
	m_Props = props;
	m_CloseCallback = callback;

	// Init information for the window that we want to create
	WNDCLASSEX wc
	{
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = OnEvent,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = APP_PROC(),
		.hIcon = LoadIcon(NULL, IDI_APPLICATION),
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH),
		.lpszClassName = m_Props.Title.c_str(),
		.hIconSm = LoadIcon(NULL, IDI_APPLICATION)
	};

	// Register the window class
	if (!RegisterClassEx(&wc))
	{
		VS_DB_OUT_W(L"Failed to register window class!\n");
		return false;
	}

	// Create the window
	m_WindowHandle = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		m_Props.Title.c_str(),
		m_Props.Title.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, m_Props.Width, m_Props.Height,
		NULL, NULL, APP_PROC(), NULL);

	if (!m_WindowHandle)
	{
		VS_DB_OUT_W(L"Failed to create window!\n");
		return false;
	}

	// Show window on screen
	ShowWindow(m_WindowHandle, SW_SHOW);
	UpdateWindow(m_WindowHandle);

	return true;
}

void Window::Destroy()
{
}

void DXH::Window::PollEvents()
{
	MSG msg;
	while (PeekMessage(&msg, m_WindowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT Window::OnEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		GetInstance().m_CloseCallback();
		GetInstance().Destroy();
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
}
