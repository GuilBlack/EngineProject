#include "InputManager.h"
#include "src/core/Window.h"

namespace DXH
{
InputManager::InputManager()
{
    // Set default keys
    std::vector<int> keys =
    {
        'W', 'A', 'S', 'D',
        VK_LBUTTON, VK_RBUTTON, VK_MBUTTON,
        VK_XBUTTON1, VK_XBUTTON2, VK_ESCAPE
    };

    for (auto key : keys)
    {
        m_KeyStates[key] = KeyState::NotUpdatedOnce;
    }
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
        switch (state)
        {
        case KeyState::JustPressed:
        case KeyState::Pressed:
            state = (GetAsyncKeyState(key)) ? KeyState::Pressed : KeyState::JustReleased;
            break;
        default:
            state = (GetAsyncKeyState(key)) ? KeyState::JustPressed : KeyState::Released;
            break;
        }
    }
}

void InputManager::SetFollowedKeys(const std::vector<int>& newKeys)
{
    auto& im = GetInstance();
    // Remove keys that are not in the new vector
    for (auto it = im.m_KeyStates.begin(); it != im.m_KeyStates.end();)
    {
        if (std::find(newKeys.begin(), newKeys.end(), it->first) == newKeys.end())
        {
            it = im.m_KeyStates.erase(it);
        }
        else ++it; // Only increment if not erased
    }

    // Add new keys
    for (auto key : newKeys)
    {
        if (im.m_KeyStates.contains(key)) continue;
        im.m_KeyStates[key] = KeyState::NotUpdatedOnce;
    }
}

void InputManager::ToggleCursorLock(bool locked)
{
    auto& im = GetInstance();
    im.m_CursorLocked = locked;
    im.Update(); // Update for catching first mouseDelta
    im.m_MouseDelta = Vector2::Zero;
    im.m_MousePosition = Vector2::Zero;
    ShowCursor(!locked);
}
}
