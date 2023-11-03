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
    inline Vector2 GetMouseDelta() const { return m_MouseDelta; }
    /// <summary>
    /// Gets the mouse actual position.
    /// </summary>
    inline Vector2 GetMousePosition() const { return m_MousePosition; }
    /// <summary>
    /// Sets the followed keys (keys that will be updated every frame).
    /// </summary>
    void SetFollowedKeys(const std::vector<int>& keys);
    /// <summary>
    /// Gets the state of the key.
    /// </summary>
    inline KeyState GetKeyState(int key) const { return m_KeyStates.at(key); }
    ///<summary>
    /// Check key if is pressed
    ///</summary> 
    inline bool IsKeyPressed(int key) const { return GetKeyState(key) == KeyState::Held || m_KeyStates.at(key) == KeyState::Pressed; }
    /// <summary>
    /// Toggle the cursor lock and visibility.
    /// </summary>
    void ToggleCursorLock(bool locked);
    void ToggleCursorLock();

private:
    Vector2 m_MouseDelta = Vector2::Zero;
    Vector2 m_MousePosition;
    std::unordered_map<int, KeyState> m_KeyStates;
    bool m_CursorLocked = false;
};
}
