#pragma once
#include "src/DXHMaths.h" // Vector2

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
public:
	InputManager();
	~InputManager();
	/// <summary>
	/// Checks for the mouse movement and updates the key states.
	/// </summary>
	void Update();
	/// <summary>
	/// Gets the mouse delta since the last update.
	/// </summary>
	inline Vector2 GetMouseDelta() const { return m_MouseDelta; }
	/// <summary>
	/// Sets the followed keys (keys that will be updated every frame).
	/// </summary>
	void SetFollowedKeys(const std::vector<int>& keys);
    /// <summary>
    /// Gets the state of the key.
    /// </summary>
    inline KeyState GetKeyState(int key) const { return m_KeyStates.at(key); }

private:
	Vector2 m_MouseDelta = Vector2::Zero;
	std::unordered_map<int, KeyState> m_KeyStates;
};
}
