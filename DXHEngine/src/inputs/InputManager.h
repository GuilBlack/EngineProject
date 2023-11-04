#pragma once
#include "DXHMaths.h" // Vector2

namespace DXH
{

enum class KeyState
{
    NotUpdatedOnce, // The key has not been updated once yet
    Released,
    Pressed,
    Held,
};

class InputManager
{
    InputManager();
    ~InputManager();
public:

    inline static InputManager& GetInstance()
    {
        static InputManager instance;
        return instance;
    }
    /// <summary>
    /// Checks for the mouse movement and updates the key states.
    /// </summary>
    void Update();
    /// <summary>
    /// Gets the mouse delta since the last update.
    /// </summary>
    inline static Vector2 GetMouseDelta() { return GetInstance().m_MouseDelta; }
    /// <summary>
    /// Gets the current mouse position.
    /// </summary>
    inline static Vector2 GetMousePosition() { return GetInstance().m_MousePosition; }
    /// <summary>
    /// Sets the followed keys (keys that will be updated every frame).
    /// </summary>
    static void SetFollowedKeys(const std::vector<int>& keys);
    /// <summary>
    /// Gets the key's current state.
    /// </summary>
    inline static KeyState GetKeyState(int key) { return GetInstance().m_KeyStates.at(key); }
    /// <summary>
    /// Checks if a key if is pressed.
    /// </summary> 
    inline static bool IsKeyPressed(int key) { return GetInstance().m_KeyStates.at(key) >= KeyState::Pressed; } // pressed = 2 & held = 3
    /// <summary>
    /// Checks if a key if is released.
    /// </summary>
    inline static bool IsKeyReleased(int key) { return GetInstance().m_KeyStates.at(key) == KeyState::Released; }
    /// <summary>
    /// Sets the cursor lock and visibility.
    /// </summary>
    static void ToggleCursorLock(bool locked);
    /// <summary>
    /// Toggle the cursor lock and visibility.
    /// </summary>
    inline static void ToggleCursorLock() { ToggleCursorLock(!GetInstance().m_CursorLocked); }

private:
    Vector2 m_MouseDelta = Vector2::Zero;
    Vector2 m_MousePosition = Vector2::Zero;
    std::unordered_map<int, KeyState> m_KeyStates;
    bool m_CursorLocked = false;
};
}
