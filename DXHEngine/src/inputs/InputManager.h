#pragma once
#include "DXHMaths.h" // Vector2

namespace DXH
{

enum class KeyState
{
    NotUpdatedOnce, // The key has not been updated once yet
    JustReleased, // First frame released
    Released,
    JustPressed, // First frame pressed
    Pressed,
};

class InputManager
{
public:
    InputManager();
    ~InputManager();

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
    inline static KeyState GetKeyState(int key) { return GetInstance().m_KeyStates.at(std::toupper(key)); }
    /// <summary>
    /// Sets the cursor lock and visibility.
    /// </summary>
    static void ToggleCursorLock(bool locked);
    /// <summary>
    /// Toggle the cursor lock and visibility.
    /// </summary>
    inline static void ToggleCursorLock() { ToggleCursorLock(!GetInstance().m_CursorLocked); }
    static Vector2 NormalizeCoordinates(Vector2 position, float screenWidth, float screebHeight);
    static Vector2 GetNormalizeCoord();
private:
    Vector2 m_MouseDelta = Vector2::Zero;
    Vector2 m_MousePosition = Vector2::Zero;
    std::unordered_map<int, KeyState> m_KeyStates;
    bool m_CursorLocked = false;
};
}
