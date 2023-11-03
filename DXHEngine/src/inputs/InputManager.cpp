#include "InputManager.h"
#include "src/core/Window.h"

namespace DXH
{
InputManager::InputManager()
{
    // Set default keys
    std::vector<int> keys =
    {
        'w', 'a', 's', 'd',
        VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
        VK_XBUTTON1, VK_XBUTTON2, VK_ESCAPE
    };
    SetFollowedKeys(keys);
}

InputManager::~InputManager()
{
}

void InputManager::Update()
{
     // Get mouse position
     POINT mousePos;
     GetCursorPos(&mousePos);
     ScreenToClient(Window::GetInstance().GetWindowHandle(), &mousePos);

    if (m_CursorLocked)
    {
        // Calculate mouse delta relative to center of screen
        POINT screenCenter
        {
            .x = Window::GetInstance().GetWidth() / 2,
            .y = Window::GetInstance().GetHeight() / 2,
        };
        m_MouseDelta.x = (float)(mousePos.x) - screenCenter.x;
        m_MouseDelta.y = screenCenter.y - (float)(mousePos.y); // Inverted, since Windows positive y is downward

        // Reset cursor position to center of screen
        ClientToScreen(Window::GetInstance().GetWindowHandle(), &screenCenter);
        SetCursorPos((int)screenCenter.x, (int)screenCenter.y);
    }
    else
    {
        m_MousePosition.x = (float)(mousePos.x);
        m_MousePosition.y = (float)(mousePos.y);
    }

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

void InputManager::ToggleCursorLock(bool locked)
{
    m_CursorLocked = locked;
    Update(); // Update for catching first mouseDelta
    m_MouseDelta = Vector2::Zero;
    m_MousePosition = Vector2::Zero;
    ShowCursor(!locked);
}
}
