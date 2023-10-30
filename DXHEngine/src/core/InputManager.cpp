#include "InputManager.h"
#include "Window.h"

namespace DXH
{
InputManager::InputManager()
{
	// Set default keys
	std::vector<int> keys =
	{
		'w', 'a', 's', 'd',
			VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
			VK_XBUTTON1, VK_XBUTTON2
	};
	SetFollowedKeys(keys);

	// Hide cursor
	ShowCursor(false);
}

InputManager::~InputManager()
{
	// Show cursor
	ShowCursor(true);
}

void InputManager::Update()
{
	// Get mouse position
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(Window::GetInstance().GetWindowHandle(), &mousePos);

	// Calculate mouse position relative to center of screen
	int screenCenterX = Window::GetInstance().GetWidth();
	int screenCenterY = Window::GetInstance().GetHeight();
	m_MouseDelta.x = mousePos.x - screenCenterX;
	m_MouseDelta.y = mousePos.y - screenCenterY;

	// Reset cursor position to center of screen
	SetCursorPos(screenCenterX, screenCenterY);

	// Get buttons
	for (auto& [key, state] : m_KeyStates)
	{
		state = GetAsyncKeyState(key) ? KeyState::Pressed : KeyState::Released;
		// TODO: Held state
	}
}

void InputManager::SetFollowedKeys(const std::vector<int>& newKeys)
{
	// Remove keys that are not in the new vector
	for (auto it = m_KeyStates.begin(); it != m_KeyStates.end();)
	{
		if (std::find(newKeys.begin(), newKeys.end(), it->first) == newKeys.end())
		{
			it = m_KeyStates.erase(it);
		}
		else ++it; // Only increment if not erased
	}

	// Add new keys
	for (auto key : newKeys)
	{
		if (m_KeyStates.contains(key)) continue;
		m_KeyStates[key] = KeyState::NotUpdatedOnce;
	}
}
}
